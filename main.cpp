/*
作者：杨洪超
邮箱：yanghongchao_it@163.com
*/

#include <QApplication>
#include <QTextCodec>
#include <QAbstractItemView>
#include <QHeaderView>

#include "adaemon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec* codec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(codec);

    Adaemon w;
    w.startDaemon();

    return a.exec();
}
