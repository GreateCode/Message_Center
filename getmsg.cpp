#include "getmsg.h"

extern QList<MsgNode*> g_msgList;
extern QList<MsgNode*> g_bakList;
static QList<MsgNode*> gs_msgList;


GetMsg::GetMsg()
{

}

int GetMsg::openXml(const QString &filename)
{
    QFile rfile(filename);   //建立指向“my.xml”文件的QFile对象

    if (!rfile.open(QIODevice::ReadOnly))
    {
        qDebug() << "get msg open err";
        return 0;  //以只读方式打开
    }

    m_doc.clear();
    if (!m_doc.setContent(&rfile))
    {
        qDebug() << "set err";
        rfile.close();
        return 0;
    }

    //将文件内容读到doc中
    rfile.close();
    return 1;
}

void GetMsg::getMsg()
{
    this->parseNode ();

    this->newInfo ();
}

void GetMsg::parseNode()
{
    QDomElement docElem = m_doc.documentElement();  //返回根元素

    QDomNode n = docElem.firstChild();   //返回根节点的第一个子节点

    gs_msgList.clear();

     while(!n.isNull())
     {
         //如果节点不为空
         if (n.isElement())  //如果节点是元素
         {
             QDomElement e = n.toElement();                 //将其转换为元素
/*
             qDebug() << qPrintable(e.tagName())            //返回元素标记
                      << qPrintable(e.attribute("id"))      //返回元素id属性的值
                      << qPrintable(e.attribute("title"))
                      << qPrintable(e.attribute("timestamp"))
                      << qPrintable(e.text());
*/
             // 获取发送时间
             long tms = e.attribute("timestamp").toLong();
             QDateTime dateTime;
             dateTime.setTime_t(tms);

             MsgNode* node = new MsgNode;
             node->id       =   e.attribute("id").toInt();
             node->title    =   e.attribute("title");
             node->timeCnt  =   e.attribute("timestamp").toLong();
             node->msgText  =   e.text();
             node->sendTime =   dateTime.toString("yyyy/MM/dd hh:mm:ss");
             node->read     =   false;
             node->sendType =   e.attribute("send_type");
             node->newsType =   e.attribute("news_type");

             if (node->newsType == "meeting")
             {
                 node->type =   'A';
             }
             else if (node->newsType == "policy")
             {
                 node->type =   'B';
             }
             else if (node->newsType == "personnel")
             {
                 node->type =   'C';
             }

             gs_msgList.append(node);
         }

         n = n.nextSibling();  //下一个兄弟节点
     }

}

void GetMsg::newInfo()
{
    // 首次刷新
    if (0 == g_msgList.count())
    {
        for (int i = 0; i < gs_msgList.count(); ++i)
        {
            gs_msgList.at(i)->isNewInfo = true;
            g_msgList.push_back(gs_msgList.at(i));

            // 对比备份信息
            for (int j = 0; j < g_bakList.count(); ++j)
            {
                if (g_bakList.at(j)->id == g_msgList.at(i)->id)
                {
                    g_msgList.at(i)->isNewInfo  =   false;
                    g_msgList.at(i)->read       =   g_bakList.at(j)->read;
                    break;
                }
            }
        }

        // 清空备份链表
        for (int i = 0; i < g_bakList.count(); ++i)
        {
            free (g_bakList.at(i));
        }
        g_bakList.clear();
        return ;
    }

    // 上次的消息全部标记为旧消息
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        g_msgList.at(i)->isNewInfo = false;
    }

    // 对比，添加新消息
    MsgNode*    pNode   =   NULL;
    MsgNode*    pNode0  =   g_msgList.at(0);
    for (int i = gs_msgList.count() - 1; i >= 0; --i)
    {
        if (gs_msgList.at(i)->timeCnt > pNode0->timeCnt
                && gs_msgList.at(i)->id > pNode0->id)
        {
            pNode = gs_msgList.at(i);
            pNode->isNewInfo = true;
            g_msgList.push_front(pNode);
        }
    }

    // 删除服务器已经删除的消息
    bool flag = false;
    for (int i = 0; i < g_msgList.count(); ++i)
    {
        flag = false;
        for (int j = 0; j < gs_msgList.count(); ++j)
        {
            if (gs_msgList.at(j)->id == g_msgList.at(i)->id)
            {
                flag = true;
                break;
            }
        }

        if (!flag)
        {
            g_msgList.removeAt(i);
        }
    }

//    qDebug () << "g_msgList: " << g_msgList.count();
}

void GetMsg::sortMsg()
{
    //    qSort (g_msgList.begin(), g_msgList.end(), this->compareTime());
}

bool GetMsg::compareTime()
{
    return true;
}
