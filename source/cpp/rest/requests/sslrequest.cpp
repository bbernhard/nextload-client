#include "sslrequest.h"
#include "../settings.h"
#include <QNetworkReply>
#include <QFile>
#include <QDebug>
#include <QSslCipher>
#include <QUuid>

SslRequest::SslRequest(QObject* parent)
    : QObject(parent),
      m_data(""),
      m_contentType("application/json")
{
    m_request = new QNetworkRequest();
    QSslConfiguration conf = m_request->sslConfiguration();

    if(ConnectionSettings::instance()->getVerifySSL())
        conf.setPeerVerifyMode(QSslSocket::VerifyPeer);
    else
        conf.setPeerVerifyMode(QSslSocket::QueryPeer);

    if(ConnectionSettings::instance()->enforceSecureCiphers()){
        //currently, the allowed ciphers are directly passed to the used
        //ciphers.
        //TODO: We should change that to only use secure ciphers
        conf.setCiphers(QSslConfiguration::supportedCiphers());
    }

    m_request->setSslConfiguration(conf);
    m_request->setRawHeader("Content-Type", "application/json");
    m_request->setRawHeader("Content-Length", QByteArray::number(m_data.size()));

    //add a unique request id to every request
    QString uuid = QUuid::createUuid().toString();
    if(uuid.length() > 2){
        m_uniqueRequestId = uuid.mid(1, uuid.length() - 2);
        m_request->setRawHeader("X-Request-Id", m_uniqueRequestId.toUtf8());
    }

    m_allowedHTTPStatusCodes.push_back(401);
    m_allowedHTTPStatusCodes.push_back(409);
}

QNetworkRequest& SslRequest::getRequest(){
    return *m_request;
}

QString SslRequest::getUniqueRequestId() const{
    return m_uniqueRequestId;
}

QByteArray SslRequest::getData() const{
    return m_data;
}

void SslRequest::setData(const QString& data){
    setData(data.toUtf8());
}

void SslRequest::setData(const QByteArray& data){
    m_data = data;
    m_request->setRawHeader("Content-Length", QByteArray::number(data.size()));
}

void SslRequest::setContentType(const QString& contentType){
    m_contentType = contentType;
    m_request->setRawHeader("Content-Type", m_contentType.toUtf8());
}

QString SslRequest::getContentType() const{
    return m_contentType;
}

void SslRequest::setAllowedHTTPStatusCodes(const QList<int>& allowedHTTPStatusCodes){
    m_allowedHTTPStatusCodes = allowedHTTPStatusCodes;
}

QList<int> SslRequest::getAllowedHTTPStatusCodes() const{
    return m_allowedHTTPStatusCodes;
}

SslRequest::~SslRequest(){
    delete m_request;
}
