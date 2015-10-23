#include "backupinfo.h"

extern QList<MsgNode*> g_msgList;

QList<MsgNode*>         g_bakList;  // 存储备份文件中的消息信息

BackupInfo::BackupInfo()
{
}

void BackupInfo::writeTask()
{
    m_doc.clear();
    m_root.clear();
    QDomProcessingInstruction instruction;
    instruction = m_doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

    m_doc.appendChild(instruction);
    m_root    =   m_doc.createElement(QObject::tr("news"));
    m_doc.appendChild(m_root);

    for (int i = 0; i < g_msgList.count(); ++i)
    {
        m_root.appendChild(this->createNode(g_msgList.at(i)));
    }

//    qDebug() << "backupinfo g_msgList.count: " << g_msgList.count();
}

// 创建一个节点
QDomElement BackupInfo::createNode(const MsgNode *pNode)
{
    QDomElement newInfo     =   m_doc.createElement (QObject::tr("new"));
    newInfo.setAttribute("id",          QString::number(pNode->id));
    newInfo.setAttribute("title",       pNode->title);
    newInfo.setAttribute("timestamp",   QString::number(pNode->timeCnt));
    newInfo.setAttribute("read",        QString::number(pNode->read));

    return newInfo;
}

void BackupInfo::parseNode()
{
    QDomElement docElem = m_doc.documentElement();  //返回根元素

    QDomNode n = docElem.firstChild();   //返回根节点的第一个子节点

    g_bakList.clear();

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
                      << qPrintable(e.attribute("read"));
*/
             // 获取发送时间
             long tms = e.attribute("timestamp").toLong();
             QDateTime dateTime;
             dateTime.setTime_t(tms);

             MsgNode* node = new MsgNode;
             node->id       =   e.attribute("id").toInt();
             node->title    =   e.attribute("title");
             node->timeCnt  =   e.attribute("timestamp").toLong();
             node->read     =   e.attribute("read").toInt() ? true : false;

             g_bakList.append(node);
         }

         n = n.nextSibling();  //下一个兄弟节点
     }
}

void BackupInfo::save(const QString &filename)
{
//    qDebug() << "save as: " << filename;
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
//        qDebug() << "写入失败" << filename;
        return ;
    }

    QTextStream out(&file);
    m_doc.save(out, 4);
    file.close();
    m_root.clear();
    m_doc.clear();
}

int BackupInfo::openXml(const QString &filename)
{
    QFile rfile(filename);   //建立指向“my.xml”文件的QFile对象

    if (!rfile.open(QIODevice::ReadOnly))
    {
        qDebug() << "bak up open err";
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
