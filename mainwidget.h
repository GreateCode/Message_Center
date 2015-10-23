#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QAction>
#include <QPoint>
#include <QMouseEvent>
#include <QSize>
#include <QProcess>
#include <QPainter>
#include <QColor>
#include <QFont>
#include <QRect>

#include "btnmodel.h"
#include "buttonlabel.h"
#include "msgwidget.h"
#include "getmsg.h"
#include "httpmsg.h"
#include "readconf.h"

namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit mainWidget(QPoint pos, QWidget *parent = 0);
    ~mainWidget();

    /// 获取本对象的位置
    QPoint  widgetPos   ();

protected:
    void paintEvent(QPaintEvent*);

private slots:
    /// 搜索
    void setCmdSearch   (bool);

    /// 全部
    void setCmdAll      (bool);

    /// OA系统
    void setCmdA        (bool);

    /// 待办系统
    void setCmdB        (bool);

    /// 监控系统
    void setCmdC        (bool);

    /// 仅看未读
    void setCmdNoRead   (int state);

    /// 开始搜索
    void setStartSearch ();

    void hideWidget     ();

private:
    void mousePressEvent(QMouseEvent*   event);
    void mouseMoveEvent (QMouseEvent*   event);

    void initUI         (void);
    void initConn       (void);
    void getAllMsgList  ();
    int  getMsgCnt      ();
    
private:
    Ui::mainWidget *ui;

    QPoint          m_dPos;
    QPoint          m_widgetPos;            /// 记忆界面的位置
    QAction*        m_searchAction;
    MsgWidget*      m_msgWidget;            /// 消息界面
};

#endif // MAINWIDGET_H
