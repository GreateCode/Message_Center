#include "pagewidget.h"

PageWidget::PageWidget(QWidget *parent)
    : QWidget(parent)
{
    m_parent    =   dynamic_cast<MsgWidget*>(parent);
    m_pageBtnColor[0]   =   QColor("#FFFFFF");
    m_pageBtnColor[1]   =   QColor("#B0E0E6");
    this->initUI();
    this->initConn();
//    this->setMaxPageNum(19);
}

PageWidget::~PageWidget()
{
    
}

void PageWidget::setMaxPageNum(int num)
{
    m_maxPageNum    =   num;
    m_curPageNum    =   0;
    memset(m_curPageRange, 0, sizeof (m_curPageRange));
    this->readyPages();

//    qDebug() << "当前页数: " << num + 1;
}

int PageWidget::getCurPageNum()
{
    return m_curPageNum;
}


void PageWidget::initUI()
{
    m_prevPageBtn   =   new QPushButton (tr ("<上一页"));
    m_prevPageBtn->setObjectName("prevPageBtn");
    m_nextPageBtn   =   new QPushButton (tr ("下一页>"));
    m_nextPageBtn->setObjectName("nextPageBtn");
    m_pageWidget    =   new QWidget;
    m_pageBtnLayout =   new QHBoxLayout (m_pageWidget);

    this->fillBackground(m_prevPageBtn, m_pageBtnColor[1]);
    this->fillBackground(m_nextPageBtn, m_pageBtnColor[1]);

    m_pageLayout    =   new QHBoxLayout (this);
    m_pageLayout->addWidget(m_prevPageBtn);
    m_pageLayout->addWidget(m_pageWidget);
    m_pageLayout->addWidget(m_nextPageBtn);

    m_sigMaper      =   new QSignalMapper (this);
}

void PageWidget::initConn()
{
    connect(m_prevPageBtn, SIGNAL (clicked ()), this, SLOT (slotPrevPage()));
    connect(m_nextPageBtn, SIGNAL (clicked ()), this, SLOT (slotNextPage()));
}

// 更新页码显示
void PageWidget::updatePageWidget()
{
    int pageNum = 0;
    for (int i = 0; i < m_pageList.count(); ++i)
    {
        pageNum =  m_pageList.at(i)->text().toInt() - 1;

        // 只显示需求区间
        if (pageNum >= m_curPageRange[0] && pageNum <= m_curPageRange[1])
        {
            m_pageList.at(i)->show();
            if (i == m_curPageNum)
            {
                this->fillBackground(m_pageList.at(i), m_pageBtnColor[0]);
            }
            else
            {
                this->fillBackground(m_pageList.at(i), m_pageBtnColor[1]);
            }
        }
        else
        {
            m_pageList.at(i)->hide();
        }
    }
}

void PageWidget::fillBackground(QWidget *w, QColor color)
{
    w->setStyleSheet(QString("#%1{background-color: %2;}").arg(w->objectName()).arg(color.name()));
}


void PageWidget::readyPages()
{
    // 清空上次的页码按钮
    for (int i = 0; i < m_pageList.count(); ++i)
    {
//        m_pageList.at(i)->hide();
        m_pageBtnLayout->removeWidget(m_pageList.at(i));
        m_pageList.at(i)->close();
    }
    m_pageList.clear();

    QPushButton*    pageBtn;
    for (int i = 0; i <= m_maxPageNum; ++i)
    {
        pageBtn =   new QPushButton(QString::number(i + 1), m_pageWidget);
        pageBtn->setObjectName(QString("page_%1_Btn").arg(i));
        pageBtn->setFixedWidth(30);
        pageBtn->hide();
        m_pageList.append(pageBtn);
        m_pageBtnLayout->addWidget(m_pageList.at(i));

        // 信号转发
        connect(pageBtn, SIGNAL(clicked()), m_sigMaper, SLOT(map()));
        m_sigMaper->setMapping(pageBtn, pageBtn->text());
    }
    connect(m_sigMaper, SIGNAL(mapped(QString)), this, SLOT (slotClickedPageBtn(QString)));

    m_curPageRange[0]   =   0;
    m_curPageRange[1]   =   qMin (m_maxPageNum, 9);

    for (int i = 0; i <= m_curPageRange[1]; ++i)
    {
        m_pageList.at(i)->show();
    }

//    qDebug() << "ready page";

    this->setCurPageNum(0);
}


void PageWidget::setCurPageNum(int num)
{
    if (num >= m_pageList.count() || num < 0)
    {
        return ;
    }

    m_pageList.at(m_curPageNum)->setEnabled(true);
    m_pageList.at(num)->setEnabled(false);

    // 设置的页码不在当前显示范围
    if (num > m_curPageRange[1] || num < m_curPageRange[0])
    {
        m_curPageRange[0]   =   num -   4;
        m_curPageRange[1]   =   num +   5;
    }

    this->updatePageWidget();
    m_curPageNum    =   num;
}


void PageWidget::slotPrevPage()
{
    if (m_curPageRange[0] > 0 && m_curPageNum == m_curPageRange[0])
    {
        m_curPageRange[0]--;
        m_curPageRange[1]--;
    }
    this->setCurPageNum(m_curPageNum - 1);
    // 设置显示消息
    m_parent->showPageMsg(m_curPageNum);
}

void PageWidget::slotNextPage()
{
    if (m_curPageRange[1] < m_maxPageNum && m_curPageNum == m_curPageRange[1])
    {
        m_curPageRange[0]++;
        m_curPageRange[1]++;
    }

    this->setCurPageNum(m_curPageNum + 1);
    // 设置显示消息
    m_parent->showPageMsg(m_curPageNum);
}

void PageWidget::slotClickedPageBtn(QString num)
{
    this->setCurPageNum(num.toInt() - 1);
    // 设置显示消息
    m_parent->showPageMsg(m_curPageNum);
}
