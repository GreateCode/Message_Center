#ifndef HTTPMSG_H
#define HTTPMSG_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QSsl>
#include <QFile>
#include <QDir>

class httpMsg : public QObject
{
    Q_OBJECT
public:
    explicit httpMsg(QObject *parent = 0);
    void startRequest(const QString url, const QString &filename); //请求链接

private:
    void initConn ();

private slots:
     //完成下载后的处理
    void httpFinished();

    //接收到数据时的处理
    void httpReadyRead();

    void slotSSLErrorDeal (const QList<QSslError> & errors);

    void errSlot (QNetworkReply::NetworkError code);

private:
    QNetworkAccessManager*  m_manager;
    QNetworkReply*          m_reply;
    QFile*                  m_file;

};

#endif // HTTPMSG_H
