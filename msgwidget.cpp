#include "msgwidget.h"

QList<MsgNode*> g_msgList;
QString MsgWidget::m_msgText = "X";
static int gs_rowHeight = 35;


MsgWidget::MsgWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle    (tr ("消息表"));
    setWindowFlags(Qt::FramelessWindowHint);

    memset(m_curPageRange, 0, sizeof (m_curPageRange));
    this->setModel          (2);
    this->setTable          ();
    this->setMsgLayout      ();
    this->initConn          ();

    m_msgWidget =   new LookOneMsg(this);
}

/// 设置最大页码和当前页面
void MsgWidget::setPageMax()
{
    int rowsHeight  =   0;
    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        rowsHeight += this->rowHeightHide(m_msgView, i);
    }

    m_pageMax = rowsHeight / (gs_rowHeight * 11);
    m_pageMax = rowsHeight % (gs_rowHeight * 11) ? m_pageMax + 1 : m_pageMax;

    /// 保存当前页码
    m_pageNum   =   m_page->getCurPageNum();

    /// 设置最大页码
    m_page->setMaxPageNum(m_pageMax - 1);

    /// 显示对应的消息页
    this->showPageMsg(m_pageNum);
}

/// 打开或关闭消息内容
void MsgWidget::openMsg(const QModelIndex& index)
{
/*
    qDebug () << "row: " << index.row()
              << " col: " << index.column()
              << " height: " << m_msgView->rowHeight(index.row());
*/

    /// 只能点击有内容的行,或 点击消息内容行，直接退出
    if (index.row() + 1 > m_model->rowCount()
            || m_model->item(index.row(), 0)->whatsThis() == this->m_msgText)
    {
        return ;
    }

    if (((index.row() + 2) <= m_model->rowCount()) &&
            m_model->item(index.row() + 1, 0)->whatsThis() == this->m_msgText)
    {
        /// 偶数次点击消息标题，则关闭消息内容
        m_model->removeRow(index.row() + 1);
        this->setPageMax();
        return;
    }

    /// 在下一行插入一行
    m_model->insertRows(index.row() + 1, 1, QModelIndex());

    /// 合并这一行的列
    m_msgView->setSpan(index.row() + 1, 0, 1, 2);

    /// 查找链表的当前消息内容
    int i = 0;
    for (i = 0; i < g_msgList.count(); ++i)
    {
        if (g_msgList.at(i)->title == m_model->item(index.row(), 0)->text()
                && g_msgList.at(i)->sendTime == m_model->item(index.row(), 1)->text())
        {
            break;
        }
    }

    m_model->setItem(index.row() + 1, 0, new QStandardItem(g_msgList.at(i)->msgText));
    g_msgList.at(i)->read = true;

    /// 设置被点击行的图标为已读
    m_model->item(index.row(), 0)->setIcon(QIcon (":/read"));

    /// 首先 本行自动调整行高
    m_msgView->resizeRowToContents(index.row() + 1);
    int rowHeight = this->rowHeightHide(m_msgView, index.row() + 1);
    rowHeight = qMax (rowHeight, gs_rowHeight);

    /// 设置本行的行高是普通行高的倍数
    m_msgView->setRowHeight(index.row() + 1, qCeil(rowHeight / gs_rowHeight) * gs_rowHeight);

    /// 自定义可删除单元格标记
    m_model->item(index.row() + 1, 0)->setWhatsThis(this->m_msgText);

    /// 消息内容的文本背景设为灰色
    m_model->item(index.row() + 1, 0)->setBackground(QBrush(QColor("#dcdcdc")));

    /// 增加了一行，要重新设置最大页码
    this->setPageMax();

    /// 本消息标记已读,颜色变灰
    m_model->item   (index.row(), 0)->setForeground(QBrush(QColor(170, 170, 170)));
}

#include <QMessageBox>

void MsgWidget::onLookOneMsg(const QModelIndex &index)
{
    /// 只能点击有内容的行,或 点击消息内容行，直接退出
    if (index.row() + 1 > m_model->rowCount()
            || m_model->item(index.row(), 0)->whatsThis() == this->m_msgText)
    {
        return ;
    }

    /// 查找链表的当前消息内容
    int i = 0;
    for (i = 0; i < g_msgList.count(); ++i)
    {
        if (g_msgList.at(i)->title == m_model->item(index.row(), 0)->text()
                && g_msgList.at(i)->sendTime == m_model->item(index.row(), 1)->text())
        {
            break;
        }
    }

    m_msgWidget->setTitle(g_msgList.at(i)->title);
    m_msgWidget->setBrowseText(g_msgList.at(i)->msgText);
    g_msgList.at(i)->read = true;

    /// 设置被点击行的图标为已读
    m_model->item(index.row(), 0)->setIcon(QIcon (":/read"));
    m_msgWidget->show();
    m_msgWidget->showNormal();
    m_msgWidget->activateWindow();

//    qDebug() << "index.row:" << index.row() << " " << g_msgList.at(i)->title;
}


/// 获取行高，不用显示行内容
int MsgWidget::rowHeightHide(QTableView* tableView, const int row)
{
    int height = 0;
    tableView->showRow(row);
    height = tableView->rowHeight(row);
    tableView->hideRow(row);

    return height;
}

/// 当前页面，此行及以上行，显示的行高度
int MsgWidget::rowsShowHeight(int curRowNum)
{
    int height = 0;
    for (int i = m_curPageRange[0]; i <= curRowNum; ++i)
    {
        height += m_msgView->rowHeight(i);
    }

    return height;
}


/// 显示第 pageNum 页消息
void MsgWidget::showPageMsg(const int pageNum)
{
    int msgsHeight = 0;                                         /// 多个信息累加的高度
    int pageHeight = gs_rowHeight * 11;    /// 一个页面消息的高度
    int curMsgsHeight = 0;                                      /// 当前页面的消息累加行高
    bool flag = false;

    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        msgsHeight += this->rowHeightHide(m_msgView, i);

        if (msgsHeight > (pageHeight * pageNum))
        {
            /// 进入当前页
            curMsgsHeight += this->rowHeightHide(m_msgView, i);
            if (curMsgsHeight <= pageHeight)
            {
                /// 显示当前页的当前消息
                m_msgView->showRow(i);

                /// 记忆当前页的起止行号
                if (flag)
                {
                    m_curPageRange[0] = i;
                    flag = true;
                }
                m_curPageRange[1] = i;
            }
            else
            {
                /// 当前页的后页的消息隐藏
                m_msgView->hideRow(i);
            }
        }
        else
        {
            /// 当前页前的页消息隐藏
            m_msgView->hideRow(i);
        }
    }

    /// 设置页码
    m_page->setCurPageNum(pageNum);
}

void MsgWidget::showStartTable()
{
    /// 设置表格的各列的宽度值,还得设置一遍，否则m_model清空后会失效
    m_msgView->setColumnWidth(0, 580);
    m_msgView->setColumnWidth(1, 135);

    int rowsHeight  =   m_model->rowCount() * gs_rowHeight;

    m_pageMax = rowsHeight / (gs_rowHeight * 11);
    m_pageMax = rowsHeight % (gs_rowHeight * 11) ? m_pageMax + 1 : m_pageMax;

    /// 设置最大页码
    m_page->setMaxPageNum(m_pageMax - 1);

//    qDebug() << "set max page :" << m_pageMax;
    m_page->setCurPageNum(0);
    m_pageNum   =   0;
    this->showPageMsg       (0);
}

/// 清除所有表格
void MsgWidget::removeAllRow()
{
    m_model->clear();
}

/// 获取消息个数
int MsgWidget::getMsgCnt()
{
    int cnt = 0;
    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        if (m_model->item(i, 0)->whatsThis() != m_msgText)
        {
            cnt++;
        }
    }
    return cnt;
}

QTableView *MsgWidget::getTableView()
{
    return this->m_msgView;
}

PageWidget *MsgWidget::getPageWidget()
{
    return this->m_page;
}

void MsgWidget::setMsgLayout()
{
    m_page          =   new PageWidget (this);
    m_splitter->setFixedSize    (710, gs_rowHeight * 11 + 2);
    m_MsgVLayout    =   new QVBoxLayout (this);
    m_MsgVLayout->addWidget     (m_splitter, 11, Qt::AlignCenter | Qt::AlignTop);
    m_MsgVLayout->addSpacing    (10);
    m_MsgVLayout->addWidget     (m_page,     1,  Qt::AlignCenter | Qt::AlignBottom);
    m_MsgVLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout             (m_MsgVLayout);
}


void MsgWidget::initConn()
{
    /// 单击表格一行响应
    connect(m_msgView,  SIGNAL (clicked(const QModelIndex&)), this, SLOT  (openMsg(const QModelIndex&)));
    connect(m_msgView,  SIGNAL (doubleClicked(const QModelIndex&)), this, SLOT(onLookOneMsg(const QModelIndex&)));
//    connect(m_msgView,  SIGNAL (activated (const QModelIndex&)), this, SLOT(onLookOneMsg(const QModelIndex&)));
}

void MsgWidget::setModel(const int num)
{
    m_model =   new QStandardItemModel;

    m_model->setColumnCount (num);
    m_model->setHeaderData  (0, Qt::Horizontal, tr ("消息"));
    m_model->setHeaderData  (1, Qt::Horizontal, tr ("时间"));
}

void MsgWidget::setTable()
{
    m_msgView   =   new QTableView;

    m_msgView->setModel         (m_model);

    // 无边框
    m_msgView->setFrameShape(QFrame::NoFrame);
    m_msgView->setShowGrid(false);

    /// 设置为可以选中多个目标
    m_msgView->setSelectionMode (QAbstractItemView::SingleSelection);

    /// 设置列宽不可变动，即不能通过鼠标拖动增加列宽
    m_msgView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_msgView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);

    /// 设置表格的各列的宽度值
    m_msgView->setColumnWidth(0, 580);
    m_msgView->setColumnWidth(1, 135);

    /// 默认显示行列头，如果你觉得不美观的话，我们可以将隐藏
    m_msgView->verticalHeader()->hide   ();
    m_msgView->horizontalHeader()->hide ();

    /// 设置选中时为整行选中
    m_msgView->setSelectionBehavior (QAbstractItemView::SelectRows);

    /// 设置表格的单元为只读属性，即不能编辑
    m_msgView->setEditTriggers      (QAbstractItemView::NoEditTriggers);

    /// 如果你用在QTableView中使用右键菜单，需启用该属性
    m_msgView->setContextMenuPolicy (Qt::CustomContextMenu);

    /// 设置鼠标跟随
    m_msgView->setMouseTracking     (true);

    m_msgView->setWindowFlags(Qt::FramelessWindowHint);

    m_splitter  =   new QSplitter;
    m_splitter->setOrientation      (Qt::Vertical);
    m_splitter->addWidget           (m_msgView);
    m_splitter->setWindowFlags(Qt::FramelessWindowHint);
//    QHBoxLayout*    layout  =   new QHBoxLayout (this);
//    layout->addWidget(m_splitter);
}

/// 添加消息条目
void MsgWidget::addMsg(const MsgNode& msgNode, const int row)
{
    /// 设置图标
    QString icon (msgNode.read ? ":/read" : ":/no_read");

    /// 设置消息头
    m_model->setItem(row, 0, new QStandardItem (QIcon (icon), msgNode.title));

    /// 设置消息发送时间
    m_model->setItem(row, 1, new QStandardItem (msgNode.sendTime));

    /// 设置文本字体颜色
    if (msgNode.read)
    {
        m_model->item   (row, 0)->setForeground(QBrush(QColor(170, 170, 170)));
    }

    /// 设置消息行高度
    m_msgView->setRowHeight(row, gs_rowHeight);
}

