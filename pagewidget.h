#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QList>
#include <QHBoxLayout>
#include <qmath.h>
#include <QSignalMapper>
#include <QLabel>
#include <QDebug>
#include <QPalette>
#include <QColor>

#include "msgwidget.h"

class MsgWidget;

class PageWidget : public QWidget
{
    Q_OBJECT
    
public:
    PageWidget(QWidget *parent = 0);
    ~PageWidget();

    void    setMaxPageNum (int num);
    int     getCurPageNum ();
    void    setCurPageNum (int num);

private:
    void    initUI          ();
    void    initConn        ();
    void    readyPages      ();
    void    updatePageWidget();
    void    fillBackground  (QWidget* w, QColor color);

private slots:
    void    slotPrevPage        ();
    void    slotNextPage        ();
    void    slotClickedPageBtn  (QString num);

private:
    MsgWidget*          m_parent;
    QList<QPushButton*> m_pageList;
    int                 m_curPageNum;   // >= 0
    int                 m_maxPageNum;
    int                 m_curPageRange[2];
    QLabel*             m_maxPageNumLabel;
    QPushButton*        m_prevPageBtn;
    QPushButton*        m_nextPageBtn;
    QPushButton*        m_manyPageBtn;
    QWidget*            m_pageWidget;
    QHBoxLayout*        m_pageBtnLayout;
    QHBoxLayout*        m_pageLayout;
    QSignalMapper*      m_sigMaper;
    QColor              m_pageBtnColor[2];
};

#endif // PAGEWIDGET_H
