#include "tipwidget.h"
#include "ui_tipwidget.h"

tipWidget::tipWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tipWidget)
{
    ui->setupUi(this);
    this->initUI();
    this->initConn();

    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
}

tipWidget::~tipWidget()
{
    delete ui;
}

void tipWidget::setTitle(const QString &title)
{
    ui->titleLabel->setText(title);
}

void tipWidget::setInfoContent(const QString &content)
{
    int lineLen = 21;
    if (content.length() <= lineLen)
    {
        ui->infoLabel->setText(content);
    }
    else
    {
        QString newContent;
        int len = content.length();
        for (int i = 0; i < len; i += lineLen)
        {
            newContent.append("<p style='margin-buttom:10px;white-space:pre'>");
            newContent.append(content.mid(i, qMin(len - lineLen * i, lineLen)));
            newContent.append("</p>");
        }

        ui->infoLabel->setText(newContent);
    }
}

void tipWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor("#E0E0E0"));
    painter.setPen(pen);
    painter.setFont(QFont("Arial", 50));
    painter.drawRect(QRect(0, 0, this->width() - 1, this->height() - 1));
}

void tipWidget::initUI()
{
    ui->closeBtn->getIconPath(":/close");
    ui->moreLabel->setLabelText("查看更多");
    ui->moreLabel->setUnderLine(true);
//    ui->infoLabel->setStyleSheet(QString("%1{font-size: 9pt;line-height:10pt;}").arg(ui->infoLabel->objectName()));
}

void tipWidget::initConn()
{
    connect(ui->closeBtn,       SIGNAL (clicked()),         this, SIGNAL(clickedClose()));
    connect(ui->moreLabel,      SIGNAL (labelClicked(bool)),this, SIGNAL(clickedMore(bool)));
}

void tipWidget::mousePressEvent(QMouseEvent *event)
{
    m_dPos    =   event->globalPos() - this->pos ();
}

void tipWidget::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - m_dPos);
}
