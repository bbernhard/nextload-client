#ifndef BASIC_REQUEST_H
#define BASIC_REQUEST_H

#include <QObject>
#include "sslrequest.h"

class BasicRequest : public SslRequest{
    Q_OBJECT
    Q_PROPERTY(QString baseUrl READ getBaseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(QString requestId READ getRequestId WRITE setRequestId NOTIFY requestIdChanged)
public:
    BasicRequest(const QString& baseUrl, const QString& requestId);
    BasicRequest();
    Q_INVOKABLE void setBaseUrl(const QString& baseUrl);
    QString getBaseUrl() const;
    Q_INVOKABLE void setRequestId(const QString& requestId);
    Q_INVOKABLE void setToken(const QString &token);
    QString getRequestId() const;
    ~BasicRequest();
protected:
    QString m_baseUrl;
    QString m_requestId;
    QString m_token;
signals:
    void baseUrlChanged();
    void requestIdChanged();
};

#endif /*BASIC_REQUEST_H*/
