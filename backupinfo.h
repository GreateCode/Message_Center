#ifndef BACKUPINFO_H
#define BACKUPINFO_H

#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomProcessingInstruction>
#include <QDomAttr>
#include <QDebug>
#include <QFile>
#include <QList>
#include <QDateTime>
#include <QTextStream>
#include <QObject>

#include "msgwidget.h"

class BackupInfo
{
public:
    BackupInfo();

    void    writeTask   ();
    void    save        (const QString& filename);

    int     openXml     (const QString& filename);
    void    parseNode   ();

private:
    QDomElement createNode(const MsgNode* pNode);

private:
    QDomDocument    m_doc;
    QDomElement     m_root;
};

#endif // BACKUPINFO_H
