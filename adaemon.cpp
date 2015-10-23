#include "adaemon.h"

extern QList<MsgNode*> g_msgList;
extern QString  g_conf;

Adaemon::Adaemon(QWidget *parent) :
    QWidget(parent)
{
}

void Adaemon::startDaemon()
{
    QString     confFile = QDir::homePath() + g_conf;
    ReadConf    readConf    (confFile);

    // 打开备份
    BackupInfo  bak;
    QFile bakFile (readConf.getBakFile());
    if (bakFile.exists())
    {
        bak.openXml(bakFile.fileName());
        bak.parseNode();
    }

    m_msgCenter     =   NULL;
    m_infoCnt       =   0;
    m_centerState   =   false;

    // 居中显示
    QDesktopWidget desktop;
    m_centerPoint = QPoint ((desktop.width() - 900) / 2, (desktop.height() - 536) / 2);

    m_infoTimer     =   new QTimer(this);
    m_infoTimer->setInterval(readConf.getLoopTime());
    m_infoTimer->start();
    m_tipTimer      =   new QTimer (this);
    m_tipTimer->setInterval(5000);
    m_bakTimer      =   new QTimer (this);
    m_bakTimer->setInterval(8000);

    this->initUI    ();
    this->initConn  ();
    this->flushServer();
    this->initDBus();
    m_bakTimer->start();
}

void Adaemon::initUI()
{
    // 右下角图标
    m_tray = new QSystemTrayIcon(this);
    m_tray->setIcon(QIcon(":/msg_tray"));
    m_tray->setToolTip(tr("消息推送"));

    QMenu  *menu    =   new QMenu;
    m_showAction    =   new QAction ("主页", menu);
    m_refreshAction =   new QAction ("刷新", menu);
    m_closeAction   =   new QAction ("退出", menu);
    menu->addAction(m_showAction);
    menu->addAction(m_refreshAction);
    menu->addAction(m_closeAction);

    m_tray->setContextMenu(menu);
    m_tray->show();

    // 消息提示框
    m_tipWidget =   new tipWidget;
    QDesktopWidget  desktop;
    m_tipWidget->move(desktop.width() - 10, desktop.height() - 100);
}

void Adaemon::initConn()
{
    connect(this,           SIGNAL(newInfo()),          this,   SLOT(showTipInfo  ()));
    connect(m_tipWidget,    SIGNAL(clickedMore(bool)),  this,   SLOT(showMsgCenter(bool)));
    connect(m_infoTimer,    SIGNAL(timeout()),          this,   SLOT(flushServer  ()));
    connect(m_tipTimer,     SIGNAL(timeout()),          this,   SLOT(closeTip     ()));
    connect(m_tipWidget,    SIGNAL(clickedClose()),     this,   SLOT(closeTip     ()));
    connect(m_showAction,   SIGNAL(triggered(bool)),    this,   SLOT(showMsgCenter(bool)));
    connect(m_refreshAction,SIGNAL(triggered()),        this,   SLOT(refreshInfo  ()));
    connect(m_closeAction,  SIGNAL(triggered()),        this,   SLOT(closeProgram ()));
    connect(m_bakTimer,     SIGNAL(timeout()),          this,   SLOT(backupMsg()));
    connect(m_tray,         SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayactivated(QSystemTrayIcon::ActivationReason)));
}

void Adaemon::initDBus()
{
    if (!QDBusConnection::sessionBus().isConnected())
    {

        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return;
    }

    if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME))
    {
        fprintf(stderr, "%s\n",
                qPrintable(QDBusConnection::sessionBus().lastError().message()));
        exit(1);
    }

    QDBusConnection::sessionBus().registerObject("/", this, QDBusConnection::ExportAllSlots);
}

// 右下角小图标操作
void Adaemon::trayactivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        m_msgCenter->show          ();
        m_msgCenter->showNormal     ();
        m_msgCenter->activateWindow ();
        m_msgCenter->move           (m_centerPoint);
    }
    else if(reason == QSystemTrayIcon::Context && !m_msgCenter->isActiveWindow())
    {
        m_showAction->setEnabled    (true);
        m_closeAction->setEnabled   (true);
        if(reason == QSystemTrayIcon::DoubleClick)
        {
            m_msgCenter->show           ();
            m_msgCenter->showNormal     ();
            m_msgCenter->activateWindow ();
            m_msgCenter->move           (m_centerPoint);
        }
        else if(reason == QSystemTrayIcon::Context && !m_msgCenter->isActiveWindow())
        {
            m_showAction->setEnabled    (true);
            m_closeAction->setEnabled   (true);
        }
    }
}

// 刷新消息
void Adaemon::flushServer()
{
    if (NULL != m_msgCenter)
    {
        // 上次的状态
        if (m_msgCenter->isHidden())
        {
            m_centerPoint   =   m_msgCenter->widgetPos();
            m_centerState =   false;
        }
        else
        {
            m_centerPoint   =   m_msgCenter->pos();
            m_centerState   =   true;
            m_msgCenter->hide();
        }

        // 关闭上次的消息表
        delete m_msgCenter;
        m_msgCenter = NULL;
    }

    m_msgCenter =   new mainWidget (m_centerPoint);

    if (m_centerState)
    {
        m_msgCenter->show();
        m_msgCenter->move(m_centerPoint);
    }
    else
    {
        m_msgCenter->hide();
    }

    // 统计新消息个数
    int cnt = 0;
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        if (g_msgList.at(i)->isNewInfo)
        {
            cnt++;
        }
    }
//    qDebug () << "cnt: " << cnt << " listCnt: " << g_msgList.count();
    m_infoCnt = cnt;
    if (cnt)
    {
        emit newInfo();
    }
}

void Adaemon::showMsgCenter(bool)
{
    if (m_msgCenter->isHidden())
    {

        m_msgCenter->show();
        m_msgCenter->move(m_centerPoint);
        m_centerState   =   true;
    }
}

void Adaemon::showTipInfo()
{
    m_tipTimer->start   ();

    m_tipWidget->setTitle       (g_msgList.at(0)->title);
    m_tipWidget->setInfoContent (g_msgList.at(0)->msgText);
    m_tipWidget->show();
    m_tipWidget->activateWindow();

    m_infoCnt = 0;

//    qDebug() << "show Tip";
}


void Adaemon::closeTip()
{
    m_tipWidget->hide   ();
    m_tipTimer->stop    ();
}

// 关闭程序
void Adaemon::closeProgram()
{
    QString     confFile = QDir::homePath() + g_conf;
    ReadConf    readConf    (confFile);

    // 写入备份
    BackupInfo  bak;
    bak.writeTask();
    bak.save(readConf.getBakFile());

    if (m_msgCenter)
    {
        delete m_msgCenter;
        m_msgCenter = NULL;
        m_centerState   =   false;
    }

    m_tipWidget->close();

    for (int i = 0; i < g_msgList.count(); ++i)
    {
        delete g_msgList.at(i);
    }
    g_msgList.clear();

    this->close();
}

// 手动刷新
void Adaemon::refreshInfo()
{
    m_infoTimer->stop();
    this->flushServer();
    m_infoTimer->start();
}

void Adaemon::backupMsg()
{
    QString     confFile = QDir::homePath() + g_conf;
    ReadConf    readConf    (confFile);

    // 写入备份
    BackupInfo  bak;
    bak.writeTask();
    bak.save(readConf.getBakFile());
}

void Adaemon::showMessageWidget()
{
    this->trayactivated(QSystemTrayIcon::DoubleClick);
}
