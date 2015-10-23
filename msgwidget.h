#ifndef MSGWIDGET_H
#define MSGWIDGET_H

#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QSplitter>
#include <QTableView>
#include <QLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QTextEdit>
#include <QDebug>
#include <QModelIndex>
#include <QList>
#include <QDateTime>
#include <QTime>
#include <QVBoxLayout>
#include <QString>
#include <QtCore/QtMath>

#include "pagewidget.h"
#include "lookonemsg.h"

class PageWidget;

/// 接收消息节点
typedef struct _MsgNode
{
    int         id;
    QString     title;      /// 消息标题
    char        type;       /// 消息种类
    QString     sendType;   /// 发送对象对应关系
    QString     newsType;   /// 信息类型对应关系
    bool        read;       /// 查看标志
    long        timeCnt;
    bool        isNewInfo;
    QString     sendTime;   /// 发送时间
    QString     curTime;    /// 查看时间
    QString     msgText;    /// 消息内容
} MsgNode;

/// 请求，all,A,B,C,D,E互斥
typedef struct _CMD
{
    QString     search; /// 搜索关键词,空为不搜索
    bool        all;    /// 全部
    bool        A;      /// OA系统
    bool        B;      /// 待办系统
    bool        C;      /// 监控系统
    bool        D;      /// 运监系统
    bool        E;      /// 其他
    bool        readFlg;/// 选中仅看未读
} Cmd;

/// 消息表类
class MsgWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MsgWidget(QWidget *parent = 0);

    /// 添加信息
    void addMsg (const MsgNode &msgNode, const int row);

    /// 显示当前页消息
    void showPageMsg (const int pageNum);

    /// 显示起始表格和页码
    void showStartTable ();

    void removeAllRow ();

    /// 获取消息个数
    int  getMsgCnt (void);

    QTableView* getTableView ();
    PageWidget* getPageWidget();

signals:

public slots:
    /// 打开或关闭消息
    void openMsg (const QModelIndex & index);
    void onLookOneMsg (const QModelIndex& index);

private:
//    void setDefaultCmd ();
    int  rowHeightHide (QTableView* tableView, const int row);
    int  rowsShowHeight(int curRowNum);  /// 当前页面，此行及以上行，显示的行高度
    void setMsgLayout ();
    void initConn ();
    void setModel (const int num);  /// 设置模型条目
    void setTable (void);           /// 设置表格
    void setPageMax ();


private:
    QStandardItemModel*     m_model;
    QTableView*             m_msgView;
    QSplitter*              m_splitter;
    int                     m_itemCnt;      /// 有内容表格行数

    int                     m_pageMax;      /// 最大页码值
    int                     m_pageNum;      /// 当前页码,从0起
    PageWidget*             m_page;         /// 页码
    QVBoxLayout*            m_MsgVLayout;
    Cmd*                    m_cmd;          /// 请求
    int                     m_curPageRange[2];  /// 记忆当前页的起止行号
    LookOneMsg*             m_msgWidget;
static QString              m_msgText;      /// 消息文本内容
};

#endif // MSGWIDGET_H
