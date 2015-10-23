#include "readconf.h"

ReadConf::ReadConf(const QString &file) :
    m_file (file)
{

}

QString ReadConf::getUrl()
{
    QFile fileConf (m_file);
    if (fileConf.open(QIODevice::ReadOnly))
    {
        QTextStream txtInput (&fileConf);
        QString lineStr;
        while (!txtInput.atEnd())
        {
            lineStr = txtInput.readLine();
            if (lineStr.contains("server"))
            {
                return lineStr.section(":::", 1, 1);
            }
        }

        fileConf.close();
    }
    return "";
}

QString ReadConf::getNewsFile()
{
    QFile fileConf (m_file);
    if (fileConf.open(QIODevice::ReadOnly))
    {
        QTextStream txtInput (&fileConf);
        QString lineStr;
        while (!txtInput.atEnd())
        {
            lineStr = txtInput.readLine();
            if (lineStr.contains("news_file"))
            {
                return lineStr.section(":::", 1, 1);
            }
        }
        fileConf.close();
    }
    return "";
}

QString ReadConf::getBakFile()
{
    QFile fileConf (m_file);
    if (fileConf.open(QIODevice::ReadOnly))
    {
        QTextStream txtInput (&fileConf);
        QString lineStr;
        while (!txtInput.atEnd())
        {
            lineStr = txtInput.readLine();
            if (lineStr.contains("bak_info"))
            {
                return QDir::homePath() + lineStr.section(":::", 1, 1);
            }
        }
        fileConf.close();
    }
    return "";
}

int ReadConf::getLoopTime()
{
    QFile fileConf (m_file);
    if (fileConf.open(QIODevice::ReadOnly))
    {
        QTextStream txtInput (&fileConf);
        QString lineStr;
        while (!txtInput.atEnd())
        {
            lineStr = txtInput.readLine();
            if (lineStr.contains("flush_time"))
            {
                return lineStr.section(":::", 1, 1).toInt();
            }
        }
        fileConf.close();
    }
    return 120000;
}
