#ifndef HTTPSREQUESTWORKER_H
#define HTTPSREQUESTWORKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QtGlobal>
#include <QUrlQuery>
#include <QThread>
#include "requests/sslrequest.h"
#include "../misc/uid.h"

enum NetworkRequestType{
    POST,
    GET,
    PUT,
    DEL,
    PROPFIND
};

struct PlainRequest{
    PlainRequest() : type(GET), retryCtr(0), disabled(false) {}
    PlainRequest(const QNetworkRequest& req) : type(GET), request(req), retryCtr(0), disabled(false) {}
    NetworkRequestType type;
    QNetworkRequest request;
    QByteArray data;
    QList<int> allowedHTTPStatusCodes;
    quint16 retryCtr;
    bool disabled;
};


class HttpsRequestWorker : public QObject{
    Q_OBJECT
public:
    HttpsRequestWorker(QObject* parent = 0);
    ~HttpsRequestWorker();
private:
    QNetworkAccessManager* m_networkManager;
    QTcpSocket* m_tcpSocket;
    quint64 m_requestId;
    QString m_username;
    QString m_password;
    QString m_baseUrl;
    quint16 m_port;
    QString m_ipAddress;
    Uid m_uid;
    QMap<quint32, PlainRequest> m_requests;

    void post(const QNetworkRequest& request, const QByteArray& data, const quint32 uid);
    void put(const QNetworkRequest& request, const QByteArray& data, const quint32 uid);
    void del(const QNetworkRequest& request, const quint32 uid);
    void get(const QNetworkRequest& request, const quint32 uid);
    void propfind(const QNetworkRequest& request, const quint32 uid);
    void markAllRequests(const bool enabled);
private slots:
    void onNetworkReply(quint32 uid);
    void onRetry(quint32 uid);
    void onRetryAll();
public slots:
    Q_INVOKABLE void get(QObject* obj);
    Q_INVOKABLE void post(QObject* obj);
    Q_INVOKABLE void put(QObject* obj);
    Q_INVOKABLE void del(QObject* obj);
    Q_INVOKABLE void propfind(QObject* obj);
signals:
    void resultReady(int reqId, QString result, int errorCode, quint32 statusCode, QString uniqueRequestId);
    void retryAll();
};

class HttpsRequestWorkerThread : public QObject{
    Q_OBJECT
public:
    HttpsRequestWorkerThread(QObject* parent = 0);
    Q_INVOKABLE void get(QObject* obj);
    Q_INVOKABLE void post(QObject* obj);
    Q_INVOKABLE void put(QObject* obj);
    Q_INVOKABLE void del(QObject* obj);
    Q_INVOKABLE void propfind(QObject* obj);
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    ~HttpsRequestWorkerThread();
private:
    HttpsRequestWorker* m_worker;
    QThread* m_thread;
private slots:
    void onResultReady(int reqId, QString result, int errorCode, quint32 statusCode, QString uniqueRequestId);
signals:
    void resultReady(int reqId, QString result, int errorCode, quint32 statusCode, QString uniqueRequestId);
};

#endif // HTTPSREQUESTWORKER_H
