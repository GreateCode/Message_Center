/*!
 * \file titlewidget.cpp
 * \brief description.
 *
 *Detailed description starts here.
 *
 * \author jun.zhang@i-soft.com.cn
 * \version 1.0
 * \date Thu Jun 25 2015 18:47:34 GMT+0800 (CST)%
 */
#include "titlewidget.h"
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>

TitleWidget::TitleWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
    initConnects();
}

void TitleWidget::initUI()
{
    m_titleLabel = new QLabel;
    m_titleLabel->setObjectName("titleLabel");
    m_closeBtn = new BtnModel;
    m_closeBtn->getIconPath (QString (":/close"));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget  (m_titleLabel,  0, Qt::AlignCenter);
    hLayout->addStretch ();
    hLayout->addWidget  (m_closeBtn,    0, Qt::AlignCenter);
    hLayout->setContentsMargins(10, 1, 1, 2);
    this->setLayout(hLayout);
}

void TitleWidget::initConnects()
{
    connect(m_closeBtn, SIGNAL(clicked()), this, SIGNAL(closeWidget()));
}

TitleWidget::~TitleWidget()
{

}

void TitleWidget::setTitleText(const QString &text, int size)
{
    m_titleLabel->setText(text);
    QFont font = m_titleLabel->font();
    font.setPointSize(size);
    m_titleLabel->setFont(font);
}

void TitleWidget::hideClose()
{
    //m_closeButton->hide();
    disconnect  (m_closeBtn,   SIGNAL(buttonClicked()), this, SIGNAL(closeWidget()));
    connect     (m_closeBtn,   SIGNAL(buttonClicked()), this, SIGNAL(hideInstall()));
}

void TitleWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/title").scaled(width(), height(), Qt::IgnoreAspectRatio,
                                                         Qt::SmoothTransformation));
}

