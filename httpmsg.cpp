#include "httpmsg.h"

httpMsg::httpMsg(QObject *parent) :
    QObject(parent)
{

}

void httpMsg::startRequest(const QString url, const QString &filename)
{
//    qDebug() << "startRequest";
    QUrl serverUrl (url);
    m_file      =   new QFile (filename);
    QNetworkRequest request;
    request.setUrl(serverUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, 0);

    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_0);
    request.setSslConfiguration(config);

//    qDebug() << "get request";
    m_reply = m_manager->get(request);

//    qDebug() << "initConn";
    this->initConn();
//    qDebug() << "endRequest";
}

void httpMsg::initConn()
{
    //下载完成后
    connect(m_reply,    SIGNAL(finished()), this,   SLOT(httpFinished()));

    //有可用数据时
    connect(m_reply,    SIGNAL(readyRead()),this,   SLOT(httpReadyRead()));

    // 错误处理
    connect(m_reply,    SIGNAL (sslErrors(const QList<QSslError>&)),
            this, SLOT (slotSSLErrorDeal(const QList<QSslError>&)));
    connect(m_reply,    SIGNAL (error(QNetworkReply::NetworkError)),
            this, SLOT (errSlot(QNetworkReply::NetworkError)));
}

void httpMsg::httpFinished()
{
    m_file->flush();
    m_file->close();
    delete m_file;

    m_reply->deleteLater();
    delete m_reply;
    m_reply = NULL;
    m_file  = NULL;
}

void httpMsg::httpReadyRead()
{
    if (m_file->open(QIODevice::WriteOnly))
    {
        qDebug() << "写入";
        //如果文件存在，则写入文件
        m_file->write(m_reply->readAll());
    }
}

void httpMsg::slotSSLErrorDeal(const QList<QSslError> &errors)
{
    QList<QSslError>::const_iterator conit = errors.begin();
    while(conit != errors.end())
    {
        qDebug()<< "ssl err: " << (*conit).errorString();
        ++conit;
    }

    return;
}

void httpMsg::errSlot(QNetworkReply::NetworkError code)
{
    qDebug() << "reply err: " << code;
}

