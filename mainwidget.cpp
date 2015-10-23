#include "mainwidget.h"
#include "ui_mainwidget.h"

extern QList<MsgNode*> g_msgList;
QString  g_conf             =   "/.config/isoft/news.ini";
static QString  gs_url      =   "https://10.238.208.133/ovd/client/news.php";
static QString  gs_newsFile =   "/tmp/news.xml";

mainWidget::mainWidget(QPoint pos, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);

    setWindowTitle(tr ("消息推送"));
    m_widgetPos = pos;

    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    this->initUI();
    this->initConn();

    this->getAllMsgList();
    this->setCmdAll(true);
}

mainWidget::~mainWidget()
{
    delete ui;
}

QPoint mainWidget::widgetPos()
{
    return this->m_widgetPos;
}

void mainWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor("#E0E0E0"));
    painter.setPen(pen);
    painter.setFont(QFont("Arial", 50));
    painter.drawRect(QRect(0, 0, this->width() - 1, this->height() - 1));
}


void mainWidget::mousePressEvent(QMouseEvent *event)
{
    m_dPos    =   event->globalPos() - this->pos ();
}

void mainWidget::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - m_dPos);
    m_widgetPos =   this->pos();
}

void mainWidget::initUI()
{
    ui->searchLineEdit->setPlaceholderText(tr ("请输入关键词"));

    ui->allBtn->setStyleSheet   ("text-align: left;");
    ui->meetBtn->setStyleSheet  ("text-align: left;");
    ui->policyBtn->setStyleSheet("text-align: left;");
    ui->personBtn->setStyleSheet("text-align: left;");

    /// 搜索按钮
    m_searchAction          =   new QAction (QIcon(":/search"), tr ("搜索"), ui->searchLineEdit);

    /// 把搜索按钮设置在右端
    ui->searchLineEdit->addAction (m_searchAction, QLineEdit::TrailingPosition);

    m_msgWidget             =   new MsgWidget;
    ui->viewLayout->addWidget   (m_msgWidget->getTableView());
    ui->pageHLayout->addWidget  (m_msgWidget->getPageWidget(), 0, Qt::AlignCenter);

    ui->retLabel->setLabelText("返回消息中心列表");
    ui->retLabel->setUnderLine(true);

    ui->personBtn->setWindowFlags(Qt::FramelessWindowHint);

    ui->closeBtn->getIconPath(":/close");

}

void mainWidget::initConn()
{
    connect(ui->closeBtn,       SIGNAL (clicked()),         this, SLOT (hideWidget()));
    connect(ui->readCheckBox,   SIGNAL (stateChanged(int)), this, SLOT (setCmdNoRead(int)));

    connect(ui->allBtn,     SIGNAL (clicked(bool)),   this, SLOT (setCmdAll(bool)));
    connect(ui->meetBtn,    SIGNAL (clicked(bool)),   this, SLOT (setCmdA(bool)));
    connect(ui->policyBtn,  SIGNAL (clicked(bool)),   this, SLOT (setCmdB(bool)));
    connect(ui->personBtn,  SIGNAL (clicked(bool)),   this, SLOT (setCmdC(bool)));

    connect(m_searchAction,     SIGNAL (triggered(bool)),   this, SLOT (setCmdSearch(bool)));
    connect(ui->searchLineEdit, SIGNAL (returnPressed()),   this, SLOT (setStartSearch()));
    connect(ui->retLabel,       SIGNAL (labelClicked(bool)),this, SLOT (setCmdAll(bool)));
}

void mainWidget::getAllMsgList()
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

//        qDebug() << "url: " << url << " newsFile: " << newsFile;
    }

    QProcess p(0);
    QString cmd = QString("wget %1 --no-check-certificate -O %2").arg(url).arg(newsFile);
    p.start(cmd);
    p.waitForStarted(1000);
    p.waitForFinished(10000);

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

/// 查看全部
void mainWidget::setCmdAll(bool)
{
    /// 取消仅看未读
    ui->readCheckBox->setCheckState(Qt::Unchecked);

    /// 清空以前的表格
    m_msgWidget->removeAllRow();

    for (int i = 0; i < g_msgList.count(); ++i)
    {
        m_msgWidget->addMsg(*g_msgList.at(i), i);
    }

    ui->msgCntLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    ui->retLabel->hide();
    ui->splitLabel->hide();
}

/// 会议通知 消息
void mainWidget::setCmdA(bool)
{
    /// 取消仅看未读
    ui->readCheckBox->setCheckState(Qt::Unchecked);

    /// 清空以前的表格
    m_msgWidget->removeAllRow();

    int num = 0;
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        if ('A' == g_msgList.at(i)->type)
        {
            m_msgWidget->addMsg(*g_msgList.at(i), num);
            num++;
        }
    }

    qDebug() << "会议通知 消息: " << num;
    ui->msgCntLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    ui->retLabel->hide();
    ui->splitLabel->hide();
}

/// 政策纪律宣贯
void mainWidget::setCmdB(bool)
{
    /// 取消仅看未读
    ui->readCheckBox->setCheckState(Qt::Unchecked);

    /// 清空以前的表格
    m_msgWidget->removeAllRow();

    int num = 0;
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        if ('B' == g_msgList.at(i)->type)
        {
            m_msgWidget->addMsg(*g_msgList.at(i), num);
            num++;
        }
    }
    ui->msgCntLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    ui->retLabel->hide();
    ui->splitLabel->hide();
}

/// 人事任免通知
void mainWidget::setCmdC(bool)
{
    /// 取消仅看未读
    ui->readCheckBox->setCheckState(Qt::Unchecked);

    /// 清空以前的表格
    m_msgWidget->removeAllRow();

    int num = 0;
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        if ('C' == g_msgList.at(i)->type)
        {
            m_msgWidget->addMsg(*g_msgList.at(i), num);
            num++;
        }
    }
    ui->msgCntLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    ui->retLabel->hide();
    ui->splitLabel->hide();
}

void mainWidget::setCmdNoRead(int state)
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
        ui->msgCntLabel->setText(tr ("共有消息%1个").arg(this->getMsgCnt()));
        m_msgWidget->showStartTable();
    }
    else if (state == Qt::Unchecked)
    {
        this->setCmdAll(true);
    }
    ui->retLabel->hide();
    ui->splitLabel->hide();
}


int mainWidget::getMsgCnt()
{
    return m_msgWidget->getMsgCnt();
}


void mainWidget::setStartSearch()
{
    this->setCmdSearch(true);
}

void mainWidget::hideWidget()
{
    m_widgetPos =   this->pos();
    this->hide();
}


void mainWidget::setCmdSearch(bool)
{
    if (ui->searchLineEdit->text().isEmpty())
    {
        return;
    }

    QString keyword (ui->searchLineEdit->text());

    qDebug () << "keyword: " << keyword;

    /// 取消仅看未读
    ui->readCheckBox->setCheckState(Qt::Unchecked);

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
    ui->msgCntLabel->setText(tr ("查找到包含\"%1\"的结果%2个").arg(keyword).arg(this->getMsgCnt()));
    m_msgWidget->showStartTable();
    ui->retLabel->show();
    ui->splitLabel->show();
}
