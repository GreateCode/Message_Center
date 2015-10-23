#ifndef GETMSG_H
#define GETMSG_H

#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QDebug>
#include <QFile>
#include <QList>
#include <QDateTime>
#include "msgwidget.h"

class GetMsg
{
public:
    GetMsg();

    int openXml (const QString& filename);
    void getMsg ();
private:
    void parseNode ();
    void newInfo ();
    void sortMsg ();
    bool compareTime ();

private:
    QDomDocument        m_doc;
};

#endif // GETMSG_H
