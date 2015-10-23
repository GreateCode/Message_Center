#include "lookonemsg.h"
#include "ui_lookonemsg.h"
#include <QDesktopWidget>
#include <QPoint>

LookOneMsg::LookOneMsg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LookOneMsg)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setWindowTitle(tr ("Message"));
    initUI();
    initConn();

    QDesktopWidget desk;
    QPoint point = QPoint((desk.width() - width()) / 2, (desk.height() - height()) / 2);
    move(point);

}

LookOneMsg::~LookOneMsg()
{
    delete ui;
}

void LookOneMsg::setTitle(const QString &title)
{
    ui->titleLabel->setText(title);
}

void LookOneMsg::setBrowseText(const QString &text)
{
    ui->textBrowser->setText(text);
}

void LookOneMsg::initUI()
{
    ui->closeBtn->getIconPath(":/close");
}

void LookOneMsg::initConn()
{
    connect(ui->closeBtn, SIGNAL(clicked()), this, SLOT(hide()));
}

void LookOneMsg::mousePressEvent(QMouseEvent *event)
{
    m_dPos    =   event->globalPos() - this->pos ();
}

void LookOneMsg::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - m_dPos);
}

void LookOneMsg::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor("#E0E0E0"));
    painter.setPen(pen);
    painter.setFont(QFont("Arial", 50));
    painter.drawRect(QRect(0, 0, this->width() - 1, this->height() - 1));
}
