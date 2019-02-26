#ifndef SSL_REQUEST_H
#define SSL_REQUEST_H

#include <QObject>
#include <QNetworkRequest>
#include <QSslConfiguration>

class SslRequest : public QObject{
    Q_OBJECT
public:
    SslRequest(QObject* parent = 0);
    virtual ~SslRequest();
    QNetworkRequest& getRequest();
    QByteArray getData() const;
    Q_INVOKABLE void setData(const QString& data);
    Q_INVOKABLE void setData(const QByteArray& data);
    Q_INVOKABLE void setAllowedHTTPStatusCodes(const QList<int>& allowedHTTPStatusCodes);
    Q_INVOKABLE QList<int> getAllowedHTTPStatusCodes() const;
    void setContentType(const QString& contentType);
    Q_INVOKABLE QString getUniqueRequestId() const;
    QString getContentType() const;
protected:
    QNetworkRequest* m_request;
    QList<int> m_allowedHTTPStatusCodes;
    QString m_contentType;
    QString m_uniqueRequestId;
private:
    QByteArray m_data;
};

#endif /*SSL_REQUEST_H*/
