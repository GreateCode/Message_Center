#ifndef BTNMODEL_H
#define BTNMODEL_H

#include <QPushButton>
#include <QMouseEvent>
#include <QEvent>
#include <QPixmap>
#include <QPainter>
#include <QVector>
#include <QWidget>
#include <QObject>

class BtnModel : public QPushButton
{
    Q_OBJECT
public:
    BtnModel(QWidget* parent = 0);

    void getIconPath        (const QString& iconPath);      //得到图片的路径名字，并且设置按钮为图片的大小
    void enterEvent         (QEvent *);                     //进入事件
    void leaveEvent         (QEvent *);                     //离开事件
    void mousePressEvent    (QMouseEvent *event);           //鼠标按下事件
    void mouseReleaseEvent  (QMouseEvent *event);           //鼠标释放事件
    void paintEvent         (QPaintEvent *);                //按钮绘图事件

private:
    enum ButtonStatus {NORMAL, HOVER, PRESS};   // 枚举按钮的几种状态
    ButtonStatus        m_status;               // 状态
    bool                m_mousePress;           // 按钮左键是否按下
    QVector<QPixmap>    m_mapVec;               // 保存图标
};

#endif // BTNMODEL_H
