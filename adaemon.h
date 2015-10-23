#ifndef ADAEMON_H
#define ADAEMON_H

#include <QObject>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QAction>
#include <QWidget>
#include <QMenu>
#include <QLabel>
#include <QHBoxLayout>
#include <QList>
#include <QDesktopWidget>
#include <QPoint>

#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>

#include "msgwidget.h"
#include "shadowwidget.h"

#include "titlewidget.h"
#include "backupinfo.h"
#include "mainwidget.h"
#include "tipwidget.h"

#define SERVICE_NAME            "org.example.QtDBus.AM"


class Adaemon : public QWidget
{
    Q_OBJECT
public:
    Adaemon(QWidget *parent = 0);
    void startDaemon();

private:
    void initUI ();
    void initConn ();

    // 初始化dbus接收端
    void initDBus();

signals:
    // 有新消息
    void newInfo ();

public slots:
    void trayactivated  (QSystemTrayIcon::ActivationReason reason);

    // 刷新消息
    void flushServer    ();

    // 显示消息主界面
    void showMsgCenter  (bool = true);
    void showTipInfo    ();
    void closeTip       ();

    // 退出软件
    void closeProgram   ();

    // 手动刷新消息
    void refreshInfo    ();
    void backupMsg      ();

    // 双击显示消息主页面
    void showMessageWidget ();

private:
    int                 m_infoCnt;          // 新消息个数
    QTimer*             m_infoTimer;        // 固定刷新计时器
    QTimer*             m_tipTimer;         // 提示框计时器
    QTimer*             m_bakTimer;         // 备份定时器
    QSystemTrayIcon*    m_tray;             // 右下角菜单
    QAction*            m_showAction;       // 显示主页
    QAction*            m_refreshAction;    // 刷新
    QAction*            m_closeAction;      // 退出程序
    ShadowWidget*       m_infoWidget;       // 提示框
    TitleWidget*        m_tipTitle;         // 提示框头
    QLabel*             m_infoLabel;        // 提示框内容
    bool                m_centerState;      // 消息推送表格状态，显示或隐
    mainWidget*         m_msgCenter;
    QPoint              m_centerPoint;      // 记忆消息推送界面的位置
    tipWidget*          m_tipWidget;
};

#endif // ADAEMON_H
