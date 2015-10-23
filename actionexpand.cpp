#include "actionexpand.h"

ActionExpand::ActionExpand(QObject *parent) :
    QAction(parent)
{
}

bool ActionExpand::event(QEvent *e)
{
    if (e->type() == QEvent::HoverEnter)
    {
//        qDebug () << "滑过";
        this->setIcon(QIcon(":/sys_white"));
    }
    else if (e->type() == QEvent::HoverLeave)
    {
        this->setIcon(QIcon (":/sys_gray"));
    }
    return true;
}
