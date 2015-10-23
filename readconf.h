#ifndef READCONF_H
#define READCONF_H

#include <QFile>
#include <QTextStream>
#include <QDir>

class ReadConf
{
public:
    ReadConf(const QString& file);
    QString getUrl ();
    QString getNewsFile ();
    QString getBakFile  ();
    int     getLoopTime ();

private:
    QString     m_file;
};

#endif // READCONF_H
