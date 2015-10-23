#include "btnmodel.h"
#include <QDebug>
#include <QDir>
#include <QSysInfo>

BtnModel::BtnModel(QWidget *parent) :
    QPushButton(parent)
{
    m_status        =   NORMAL;
    m_mousePress    =   false;
}

/// 获取图标
void BtnModel::getIconPath(const QString& iconPath)
{
    m_mapVec.append(QPixmap(iconPath + QString ("_normal")));
    m_mapVec.append(QPixmap(iconPath + QString ("_hover")));
    m_mapVec.append(QPixmap(iconPath + QString ("_press")));

    this->setFixedSize(m_mapVec[0].size());//设置按钮与图片一样的大小
}

/// 滑入
void BtnModel::enterEvent(QEvent *)
{
    m_status    =   HOVER;
    update();
}

/// 滑出
void BtnModel::leaveEvent(QEvent *)
{
    m_status    =   NORMAL;
    update();
}

/// 点击
void BtnModel::mousePressEvent(QMouseEvent *event)
{
    /// 若点击鼠标左键
    if (event->button() == Qt::LeftButton)
    {
        m_mousePress    =   true;
        m_status        =   PRESS;
        update();
    }
}

/// 点击后的放手
void BtnModel::mouseReleaseEvent(QMouseEvent *event)
{
    /// 若点击鼠标左键响应
    if (m_mousePress && this->rect().contains(event->pos()))
    {
        m_mousePress    =   false;
        m_status        =   HOVER;
        update();
        emit clicked();
    }
}

/// 显示效果
void BtnModel::paintEvent(QPaintEvent *)
{
    QPainter    painter (this);
    QPixmap     pixmap;

    switch(m_status)
    {
    case NORMAL:    /// 正常
    {
        pixmap = m_mapVec[0];
        break;
    }
    case HOVER:     /// 滑过
    {
        pixmap = m_mapVec[1];
        break;
    }
    case PRESS:     /// 点击
    {
        pixmap = m_mapVec[2];
        break;
    }
    default:
        pixmap = m_mapVec[0];
        break;
    }

    /// 显示当前图标
    painter.drawPixmap(rect(), pixmap);
}
