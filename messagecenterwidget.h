#ifndef MESSAGECENTERWIDGET_H
#define MESSAGECENTERWIDGET_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QPoint>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QAction>
#include <QList>
#include <QMenu>
#include <QPalette>
#include <QWidgetAction>
#include <QProcess>
#include <QDesktopWidget>

#include "btnmodel.h"
#include "buttonlabel.h"
#include "msgwidget.h"
#include "titlewidget.h"
#include "getmsg.h"
#include "httpmsg.h"
#include "readconf.h"
#include "actionexpand.h"
//#include "mainwidget.h"

class MessageCenterWidget : public QWidget
{
    Q_OBJECT

public:
    MessageCenterWidget (QPoint pos, QWidget *parent = 0);
    ~MessageCenterWidget();

    /// 获取本对象的位置
    QPoint  widgetPos   ();

protected:
    void paintEvent(QPaintEvent* e);

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

    /// 运监系统
    void setCmdD        (bool);

    /// 其他
    void setCmdE        (bool);

    /// 仅看未读
    void setCmdNoRead   (int state);

    /// 开始搜索
    void setStartSearch ();

    void hoveredAcvtion (QAction *a);

    void hideWidget     ();

private:
    void mousePressEvent(QMouseEvent*   event);
    void mouseMoveEvent (QMouseEvent*   event);

    void initUI         (void);
    void initUILeft     (void);
    void initUIRight    (void);
    void initConn       (void);
    void fillBackground (QWidget* w, QColor color);

    void getAllMsgList  ();
    int  getMsgCnt      ();

private:
//    mainWidget*     m_mainWidget;
    QPoint          m_dPos;
    QPoint          m_widgetPos;            /// 记忆界面的位置
    TitleWidget*    m_titleWidget;
    BtnModel*       m_closeBtn;             /// 关闭
    QLineEdit*      m_searchLE;             /// 搜索
    QAction*        m_searchAction;         /// 搜索事件
    QAction*        m_totalAction;          /// 全部
    QAction*        m_OASysAction;          /// OA系统
    QAction*        m_toBeDoneSysAction;    /// 待办系统
    QAction*        m_monSysAction;         /// 监控系统
    QAction*        m_transSysAction;       /// 运监系统
    QAction*        m_otherAction;          /// 其他
    QMenu*          m_sysMenu;              /// 系统菜单

    QLabel*         m_msgInfoLabel;         /// 消息个数信息
    ButtonLabel*    m_retMsgCenter;         /// 返回消息中心列表
    QCheckBox*      m_noReadCheckBox;       /// 仅看未读
    MsgWidget*      m_msgWidget;            /// 消息界面

    QHBoxLayout*    m_topHLayout;           /// 界面顶端一行
    QVBoxLayout*    m_toolVLayout;          /// 工具列
    QHBoxLayout*    m_infoHLayout;          /// 信息行
    QVBoxLayout*    m_showVLayout;          /// 显示结果
    QHBoxLayout*    m_handleHLayout;        /// 操作部分
    QVBoxLayout*    m_mainLayout;           /// 主布局
};

#endif // MESSAGECENTERWIDGET_H
