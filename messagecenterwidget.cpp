#include "messagecenterwidget.h"
#include <QSize>

extern QList<MsgNode*> g_msgList;
QString  g_conf             =   "/.config/isoft/news.ini";
static QString  gs_url      =   "https://10.238.208.133/ovd/client/news.php";
static QString  gs_newsFile =   "/tmp/news.xml";

MessageCenterWidget::MessageCenterWidget(QPoint pos, QWidget *parent)
    : QWidget(parent)
{

    setWindowTitle(tr ("消息推送"));
    m_widgetPos = pos;

    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool);

    this->initUI();
    this->initConn();

    this->getAllMsgList();
    this->setCmdAll(true);
    this->resize(900, 536);

    this->setObjectName("mainWidget");
    this->setStyleSheet("#mainWidget{background-color:#F8F8F8;}");
}

MessageCenterWidget::~MessageCenterWidget()
{
}

QPoint MessageCenterWidget::widgetPos()
{
    return m_widgetPos;
}

void MessageCenterWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen("#E0E0E0");
    painter.setFont(QFont("Arial", 1));
    painter.drawRect(QRect(0, 0, this->width() -1, this->height() -1));
}

/// 搜索
void MessageCenterWidget::setCmdSearch(bool)
{
    if (m_searchLE->text().isEmpty())
    {
        return;
    }

    QString keyword (m_searchLE->text());

    qDebug () << "keyword: " << keyword;

    /// 取消仅看未读
    m_noReadCheckBox->setCheckState(Qt::Unchecked);

    /// 清空以前的表格
    m_msgWidget->removeAllRow();

    int num = 0;
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        if (g_msgList.at(i)->title.contains(keyword) > 0)
        {
            m_msgWidget->addMsg(*g_msgList.at(i), num++);
        }
    }
    m_msgInfoLabel->setText(tr ("查找到包含\"%1\"的结果%2个").arg(keyword).arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    m_retMsgCenter->show();
}

/// 查看全部
void MessageCenterWidget::setCmdAll(bool)
{
    /// 取消仅看未读
    m_noReadCheckBox->setCheckState(Qt::Unchecked);

    /// 清空以前的表格
    m_msgWidget->removeAllRow();

    for (int i = 0; i < g_msgList.count(); ++i)
    {
        m_msgWidget->addMsg(*g_msgList.at(i), i);
    }
    m_msgInfoLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    m_retMsgCenter->hide();
}

/// 查看OA系统 消息
void MessageCenterWidget::setCmdA(bool)
{
    /// 取消仅看未读
    m_noReadCheckBox->setCheckState(Qt::Unchecked);

    /// 清空以前的表格
    m_msgWidget->removeAllRow();

    int num = 0;
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        if ('A' == !g_msgList.at(i)->type)
        {
            m_msgWidget->addMsg(*g_msgList.at(i), num);
            num++;
        }
    }
    m_msgInfoLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    m_retMsgCenter->hide();
}

/// 待办系统
void MessageCenterWidget::setCmdB(bool)
{
    /// 取消仅看未读
    m_noReadCheckBox->setCheckState(Qt::Unchecked);

    /// 清空以前的表格
    m_msgWidget->removeAllRow();

    int num = 0;
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        if ('B' == !g_msgList.at(i)->type)
        {
            m_msgWidget->addMsg(*g_msgList.at(i), num);
            num++;
        }
    }
    m_msgInfoLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    m_retMsgCenter->hide();
}

/// 监控系统
void MessageCenterWidget::setCmdC(bool)
{
    /// 取消仅看未读
    m_noReadCheckBox->setCheckState(Qt::Unchecked);

    /// 清空以前的表格
    m_msgWidget->removeAllRow();

    int num = 0;
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        if ('C' == !g_msgList.at(i)->type)
        {
            m_msgWidget->addMsg(*g_msgList.at(i), num);
            num++;
        }
    }
    m_msgInfoLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    m_retMsgCenter->hide();
}

/// 运监系统
void MessageCenterWidget::setCmdD(bool)
{
    /// 取消仅看未读
    m_noReadCheckBox->setCheckState(Qt::Unchecked);

    /// 清空以前的表格
    m_msgWidget->removeAllRow();

    int num = 0;
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        if ('D' == !g_msgList.at(i)->type)
        {
            m_msgWidget->addMsg(*g_msgList.at(i), num);
            num++;
        }
    }
    m_msgInfoLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    m_retMsgCenter->hide();
}

/// 其他
void MessageCenterWidget::setCmdE(bool)
{
    /// 取消仅看未读
    m_noReadCheckBox->setCheckState(Qt::Unchecked);

    /// 清空以前的表格
    m_msgWidget->removeAllRow();

    int num = 0;
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        if ('E' == !g_msgList.at(i)->type)
        {
            m_msgWidget->addMsg(*g_msgList.at(i), num);
            num++;
        }
    }
    m_msgInfoLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    m_retMsgCenter->hide();
}

/// 仅看未读
void MessageCenterWidget::setCmdNoRead(int state)
{
    qDebug() << "ReadState: " << state;
    int num = 0;

    if (state == Qt::Checked)
    {
        /// 清空以前的表格
        m_msgWidget->removeAllRow();
        for (int i = 0; i < g_msgList.count(); ++i)
        {
            if (!g_msgList.at(i)->read)
            {
                m_msgWidget->addMsg(*g_msgList.at(i), num);
                num++;
            }
        }
        m_msgInfoLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
        m_msgWidget->showStartTable();
    }
    else if (state == Qt::Unchecked)
    {
        this->setCmdAll(true);
    }
    m_retMsgCenter->hide();
}

void MessageCenterWidget::setStartSearch()
{
    this->setCmdSearch(true);
}

void MessageCenterWidget::hoveredAcvtion(QAction *a)
{
    a->setIcon(QIcon (":/sys_white"));
}

void MessageCenterWidget::hideWidget()
{
    m_widgetPos =   this->pos();
    this->hide();
}

void MessageCenterWidget::mousePressEvent(QMouseEvent *event)
{
    m_dPos    =   event->globalPos() - this->pos ();
}

void MessageCenterWidget::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - m_dPos);
    m_widgetPos =   this->pos();
}

void MessageCenterWidget::initUI()
{
    /// 头
    m_titleWidget           =   new TitleWidget;
    m_titleWidget->setTitleText(tr ("消息推送"), 12);
    m_titleWidget->setFixedHeight(38);

    /// 左边部分
    this->initUILeft();

    /// 右边部分
    this->initUIRight();

    /// 操作部分
    m_handleHLayout         =   new QHBoxLayout;
    m_handleHLayout->addLayout(m_toolVLayout, 2);      /// 左边工具
    m_handleHLayout->addLayout(m_showVLayout, 8);      /// 右边显示
    m_handleHLayout->setContentsMargins(0, 0, 0, 0);

    /// 主布局
    m_mainLayout            =   new QVBoxLayout (this);
    m_mainLayout->addWidget(m_titleWidget,  2, Qt::AlignTop);
    m_mainLayout->addLayout(m_handleHLayout,10);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
}

/// 初始化左边界面
void MessageCenterWidget::initUILeft()
{
    /************* 左边功能列 *****************/
    /// 搜索输入框
    m_searchLE              =   new QLineEdit;
    m_searchLE->setAlignment        (Qt::AlignRight);
    m_searchLE->setPlaceholderText  (tr ("请输入关键词"));
    m_searchLE->setWindowIcon(QIcon (":/searchEdit"));
    m_searchLE->setFixedSize        (160, 26);
    QFont searchFont        =   m_searchLE->font();
    searchFont.setPointSize (10);
    m_searchLE->setFont     (searchFont);

    /// 搜索按钮
    m_searchAction          =   new QAction (QIcon(":/search"), tr ("搜索"), m_searchLE);

    /// 把搜索按钮设置在右端
    m_searchLE->addAction (m_searchAction, QLineEdit::TrailingPosition);

    QString actionIcon (":/sys_gray");

    m_sysMenu               =   new QMenu;
    m_totalAction           =   new QAction(QIcon(actionIcon),  tr ("全部"),    m_sysMenu);
    m_OASysAction           =   new QAction (QIcon(actionIcon), tr ("OA系统"),  m_sysMenu);
    m_toBeDoneSysAction     =   new QAction (QIcon(actionIcon), tr ("待办系统"), m_sysMenu);
    m_monSysAction          =   new QAction (QIcon(actionIcon), tr ("监控系统"), m_sysMenu);
    m_transSysAction        =   new QAction (QIcon(actionIcon), tr ("运监系统"), m_sysMenu);
    m_otherAction           =   new QAction (QIcon(actionIcon), tr ("其他"),    m_sysMenu);

    m_sysMenu->addAction (m_totalAction);
    m_sysMenu->addSeparator();
    m_sysMenu->addAction (m_OASysAction);
    m_sysMenu->addSeparator();
    m_sysMenu->addAction (m_toBeDoneSysAction);
    m_sysMenu->addSeparator();
    m_sysMenu->addAction (m_monSysAction);
    m_sysMenu->addSeparator();
    m_sysMenu->addAction (m_transSysAction);
    m_sysMenu->addSeparator();
    m_sysMenu->addAction (m_otherAction);

    QFont sysFont         =   m_sysMenu->font();
    sysFont.setPointSize(10);
    m_sysMenu->setFont(sysFont);
    m_sysMenu->setFixedSize(160, 34 * 6);
    m_sysMenu->setObjectName("mymenu");

    /// 设置Action高度
    m_sysMenu->setStyleSheet("QMenu::item {padding: 8px 10px 8px 10px;}"
                             "QMenu::item:selected {border-color: white;background: rgb(53, 120, 191, 150);color:white;}");


    m_toolVLayout           =   new QVBoxLayout;
//    m_toolVLayout->addSpacing(7);
    m_toolVLayout->addWidget(m_searchLE,    0, Qt::AlignTop);
    m_toolVLayout->addSpacing(2);
    m_toolVLayout->addWidget(m_sysMenu);
    m_toolVLayout->addSpacing(237);
    m_toolVLayout->setContentsMargins(20, 7, 1, 1);
}

/// 初始化右边界面
void MessageCenterWidget::initUIRight()
{
    /***************** 右边显示区 **********************/
    /// 消息个数
    m_msgInfoLabel          =   new QLabel;
    m_msgInfoLabel->setText(tr ("查找到包含“集团”的结果 0 个"));
    QFont msgFont           =   m_msgInfoLabel->font();
    msgFont.setPointSize(10);
    m_msgInfoLabel->setFont(msgFont);

    /// 返回消息中心列表
    m_retMsgCenter          =   new ButtonLabel;
    m_retMsgCenter->setLabelText(tr ("返回消息中心列表"));
    m_retMsgCenter->setUnderLine(true);
    QFont retFont           =   m_retMsgCenter->font();
    retFont.setPointSize(10);
    m_retMsgCenter->setFont(retFont);

    /// 仅看未读复选框
    m_noReadCheckBox        =   new QCheckBox (tr ("仅看未读"));
    m_noReadCheckBox->setChecked(false);
    QFont boxFont           =   m_noReadCheckBox->font();
    boxFont.setPointSize(10);
    m_noReadCheckBox->setFont(boxFont);

    /// 信息行
    m_infoHLayout           =   new QHBoxLayout;
    m_infoHLayout->addSpacing(10);
    m_infoHLayout->addWidget(m_msgInfoLabel,    0, Qt::AlignLeft);
    m_infoHLayout->addWidget(m_retMsgCenter);
    m_infoHLayout->addWidget(m_noReadCheckBox,  0, Qt::AlignRight);
    m_infoHLayout->addSpacing(5);
    m_infoHLayout->setContentsMargins(1, 0, 10, 1);

    /// 消息界面
    m_msgWidget             =   new MsgWidget;

    /// 显示区
    m_showVLayout           =   new QVBoxLayout;
    m_showVLayout->addLayout(m_infoHLayout, 1);
    m_showVLayout->addWidget(m_msgWidget,   10, Qt::AlignCenter | Qt::AlignBottom);
    m_showVLayout->setContentsMargins(1, 7, 1, 20);
}

void MessageCenterWidget::initConn()
{
    connect(m_titleWidget,      SIGNAL (closeWidget()),     this, SLOT (hideWidget()));
    connect(m_noReadCheckBox,   SIGNAL (stateChanged(int)), this, SLOT (setCmdNoRead(int)));
    connect(m_totalAction,      SIGNAL (triggered(bool)),   this, SLOT (setCmdAll(bool)));
    connect(m_OASysAction,      SIGNAL (triggered(bool)),   this, SLOT (setCmdA(bool)));
    connect(m_toBeDoneSysAction,SIGNAL (triggered(bool)),   this, SLOT (setCmdB(bool)));
    connect(m_monSysAction,     SIGNAL (triggered(bool)),   this, SLOT (setCmdC(bool)));
    connect(m_transSysAction,   SIGNAL (triggered(bool)),   this, SLOT (setCmdD(bool)));
    connect(m_otherAction,      SIGNAL (triggered(bool)),   this, SLOT (setCmdE(bool)));
    connect(m_searchAction,     SIGNAL (triggered(bool)),   this, SLOT (setCmdSearch(bool)));
    connect(m_searchLE,         SIGNAL (returnPressed()),   this, SLOT (setStartSearch()));
    connect(m_retMsgCenter,     SIGNAL (labelClicked(bool)),this, SLOT (setCmdAll(bool)));

//    connect(m_totalAction,      SIGNAL (hovered ()),        this, SLOT (hoveredAcvtion()));
}

/// 设置背景颜色
void MessageCenterWidget::fillBackground(QWidget *w, QColor color)
{
    w->setAutoFillBackground(true);
    QPalette    p;
    p.setColor(QPalette::Background, color);
    w->setPalette(p);
}

void MessageCenterWidget::getAllMsgList()
{
    // 获得网址和消息存储文件名
    QString newsFile;
    QString url;
    QString confFile = QDir::homePath() + g_conf;
    QFile fileConf (confFile);
    if (!fileConf.exists())
    {
        newsFile    =   gs_newsFile;
        url         =   gs_url;
    }
    else
    {
        ReadConf    readConf (confFile);
        url     =   readConf.getUrl();
        newsFile=   readConf.getNewsFile();

        qDebug() << "url: " << url << " newsFile: " << newsFile;
    }

    QProcess p(0);
    QString cmd = QString("wget %1 --no-check-certificate -O %2").arg(url).arg(newsFile);
    p.start(cmd);
    p.waitForStarted(2000);
    p.waitForFinished(5000);

    qDebug() << "wget";

    // https 获取网页消息
//    httpMsg httpMsg;
//    httpMsg.startRequest(url, newsFile);

    QFile file (newsFile);
    if (!file.exists())
    {
        qDebug() << newsFile << "no exist";
        return ;
    }
    // 获取消息
    GetMsg getMsg;
    getMsg.openXml  (newsFile);
    getMsg.getMsg   ();
}

// 获取消息个数
int MessageCenterWidget::getMsgCnt()
{
    return m_msgWidget->getMsgCnt();
}
