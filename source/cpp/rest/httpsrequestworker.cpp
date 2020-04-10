#include "httpsrequestworker.h"
#include <QTimer>

const quint32 g_retryInterval = 2; //in seconds
const quint32 g_maxRetries = 3;

HttpsRequestWorker::HttpsRequestWorker(QObject* parent)
    : QObject(parent),
      m_requestId(0),
      m_port(1345),
      m_ipAddress("192.168.2.10")
{
    QObject::connect(this, SIGNAL(retryAll()), this, SLOT(onRetryAll()), Qt::QueuedConnection);
    m_baseUrl = QString("https://") + m_ipAddress + QString(":") + QString::number(m_port) + QString("/");
    m_networkManager = new QNetworkAccessManager(this);
}

void HttpsRequestWorker::get(const QNetworkRequest& request, const quint32 uid){
    QNetworkReply* reply = m_networkManager->get(request);
    if(reply){
        QObject::connect(reply, &QNetworkReply::finished, this, [=](){ onNetworkReply(uid); }, Qt::QueuedConnection);
    }
}

void HttpsRequestWorker::get(QObject* obj){
    SslRequest* req = qobject_cast<SslRequest*>(obj);
    if(req){
        quint32 uid = m_uid.next();
        PlainRequest plainRequest(req->getRequest());
        plainRequest.type = NetworkRequestType::GET;
        plainRequest.allowedHTTPStatusCodes = req->getAllowedHTTPStatusCodes();
        m_requests[uid] = plainRequest;

        QNetworkReply* reply = m_networkManager->get(req->getRequest());
        if(reply){
            qDebug() << "request: " << req->getRequest().url();
            QObject::connect(reply, &QNetworkReply::finished, this, [=](){ onNetworkReply(uid); }, Qt::QueuedConnection);
        }
    }
}

void HttpsRequestWorker::post(QObject* obj){
    SslRequest* req = qobject_cast<SslRequest*>(obj);
    if(req){
        quint32 uid = m_uid.next();
        PlainRequest plainRequest(req->getRequest());
        plainRequest.type = NetworkRequestType::POST;
        plainRequest.allowedHTTPStatusCodes = req->getAllowedHTTPStatusCodes();
        plainRequest.data = req->getData();

        m_requests[uid] = plainRequest;

        QNetworkReply* reply = m_networkManager->post(req->getRequest(), req->getData());
        if(reply){
            qDebug() << "request: " << req->getRequest().url();
            QObject::connect(reply, &QNetworkReply::finished, this, [=](){ onNetworkReply(uid); }, Qt::QueuedConnection);
        }
    }
}

void HttpsRequestWorker::post(const QNetworkRequest& request, const QByteArray& data, const quint32 uid){
    QNetworkReply* reply = m_networkManager->post(request, data);
    if(reply){
        QObject::connect(reply, &QNetworkReply::finished, this, [=](){ onNetworkReply(uid); }, Qt::QueuedConnection);
    }
}


void HttpsRequestWorker::put(QObject* obj){
    SslRequest* req = qobject_cast<SslRequest*>(obj);
    if(req){
        quint32 uid = m_uid.next();
        PlainRequest plainRequest(req->getRequest());
        plainRequest.type = NetworkRequestType::PUT;
        plainRequest.allowedHTTPStatusCodes = req->getAllowedHTTPStatusCodes();
        plainRequest.data = req->getData();

        m_requests[uid] = plainRequest;

        QNetworkReply* reply = m_networkManager->put(req->getRequest(), req->getData());
        if(reply){
            qDebug() << "request: " << req->getRequest().url();
            QObject::connect(reply, &QNetworkReply::finished, this, [=](){ onNetworkReply(uid); }, Qt::QueuedConnection);
        }
    }
}

void HttpsRequestWorker::put(const QNetworkRequest& request, const QByteArray& data, const quint32 uid){
    QNetworkReply* reply = m_networkManager->put(request, data);
    if(reply){
        QObject::connect(reply, &QNetworkReply::finished, this, [=](){ onNetworkReply(uid); }, Qt::QueuedConnection);
    }
}

void HttpsRequestWorker::del(QObject* obj){
    SslRequest* req = qobject_cast<SslRequest*>(obj);
    if(req){
        quint32 uid = m_uid.next();
        PlainRequest plainRequest(req->getRequest());
        plainRequest.type = NetworkRequestType::DEL;
        plainRequest.allowedHTTPStatusCodes = req->getAllowedHTTPStatusCodes();

        m_requests[uid] = plainRequest;

        QNetworkReply* reply = m_networkManager->deleteResource(req->getRequest());
        if(reply){
            qDebug() << "request: " << req->getRequest().url();
            QObject::connect(reply, &QNetworkReply::finished, this, [=](){ onNetworkReply(uid); }, Qt::QueuedConnection);
        }
    }
}

void HttpsRequestWorker::del(const QNetworkRequest& request, const quint32 uid){
    QNetworkReply* reply = m_networkManager->deleteResource(request);
    if(reply){
        QObject::connect(reply, &QNetworkReply::finished, this, [=](){ onNetworkReply(uid); }, Qt::QueuedConnection);
    }
}

void HttpsRequestWorker::propfind(QObject* obj){
    SslRequest* req = qobject_cast<SslRequest*>(obj);
    if(req){
        quint32 uid = m_uid.next();
        PlainRequest plainRequest(req->getRequest());
        plainRequest.type = NetworkRequestType::PROPFIND;
        plainRequest.allowedHTTPStatusCodes = req->getAllowedHTTPStatusCodes();
        plainRequest.data = req->getData();

        m_requests[uid] = plainRequest;

        QNetworkReply* reply = m_networkManager->sendCustomRequest(req->getRequest(), "PROPFIND");
        if(reply){
            qDebug() << "request: " << req->getRequest().url();
            QObject::connect(reply, &QNetworkReply::finished, this, [=](){ onNetworkReply(uid); }, Qt::QueuedConnection);
        }
    }
}

void HttpsRequestWorker::propfind(const QNetworkRequest& request, const quint32 uid){
    QNetworkReply* reply = m_networkManager->sendCustomRequest(request, "PROPFIND");
    if(reply){
        QObject::connect(reply, &QNetworkReply::finished, this, [=](){ onNetworkReply(uid); }, Qt::QueuedConnection);
    }
}

void HttpsRequestWorker::onRetry(const quint32 uid){
    PlainRequest plainRequest = m_requests[uid];
    if(plainRequest.type == NetworkRequestType::POST)
        post(plainRequest.request, plainRequest.data, uid);
    else if(plainRequest.type == NetworkRequestType::GET)
        get(plainRequest.request, uid);
    else if(plainRequest.type == NetworkRequestType::PUT)
        put(plainRequest.request, plainRequest.data, uid);
    else if(plainRequest.type == NetworkRequestType::DEL)
        del(plainRequest.request, uid);
    else if(plainRequest.type == NetworkRequestType::PROPFIND)
        propfind(plainRequest.request, uid);
}

void HttpsRequestWorker::onNetworkReply(quint32 uid){
    int requestId = -1;
    bool conversionOk = false;
    QString uniqueRequestId = "";

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if(reply){
        if(m_requests.contains(uid)){
            if(reply->hasRawHeader("X-Request-Id"))
                uniqueRequestId = reply->rawHeader("X-Request-Id");

            PlainRequest& plainRequest = m_requests[uid];

            if(plainRequest.disabled == false){
                quint32 statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

                //check if we need to retry
                //we do not need to retry if everything went well. We are also not retrying
                //for all the HTTP status codes in the provided list (~ exception list)
                if((reply->error() != QNetworkReply::NoError) &&
                        (!plainRequest.allowedHTTPStatusCodes.contains(statusCode))){
                    plainRequest.retryCtr ++;
                    if(plainRequest.retryCtr >= g_maxRetries){
                        qDebug() << "Retry max reached...aborting request";

                        //m_requests.clear();
                        markAllRequests(false);
                        qDebug() << "Marking all requests as disabled";

                        QString reqId = reply->rawHeader("X-Request-Type");
                        requestId = reqId.toInt(&conversionOk);

                        emit resultReady(requestId, "", -1, statusCode, uniqueRequestId); //emit result ready with error code -1
                        //in order to mark that the request was
                        //aborted due to max retries reached
                    }
                    else{
                        qDebug() << "Retrying request with uid: " << uid << "..." << statusCode;
                        QTimer::singleShot((g_retryInterval * 1000), this, [=](){ onRetry(uid); });
                    }
                }

                //if everything went fine, emit signal
                //and remove from requests map
                else{
                    //remove from requests map
                    if(m_requests.contains(uid))
                        m_requests.remove(uid);

                    QString data = (QString) reply->readAll();
                    if(reply->hasRawHeader("X-Request-Type")){
                        QString reqId = reply->rawHeader("X-Request-Type");
                        requestId = reqId.toInt(&conversionOk);
                    }

                    emit resultReady(requestId, data.toUtf8(), reply->error(), statusCode, uniqueRequestId);
                }
            }
            reply->deleteLater();
        }
    }
}

void HttpsRequestWorker::markAllRequests(const bool enabled){
    for(QMap<quint32, PlainRequest>::iterator it(m_requests.begin()); it != m_requests.end(); ++it){
        it->disabled = !enabled;
    }
}

void HttpsRequestWorker::onRetryAll(){
    QMap<quint32, PlainRequest> temp;
    quint32 uid;
    temp = m_requests;
    m_requests.clear();

    //enable all requests again, create a new uid for every request (to make sure, that
    //"older" responses that are just arriving late, dond't match) and retry every request
    for(QMap<quint32, PlainRequest>::iterator it(temp.begin()); it != temp.end(); ++it){
        uid = m_uid.next();
        m_requests[uid] = it.value();
        m_requests[uid].disabled = false;
        QTimer::singleShot((g_retryInterval * 1000), this, [=](){ onRetry(uid); });
    }
}

HttpsRequestWorker::~HttpsRequestWorker()
{
    if(m_networkManager)
        delete m_networkManager;
}



HttpsRequestWorkerThread::HttpsRequestWorkerThread(QObject *parent)
    : QObject(parent)
{
    m_worker = new HttpsRequestWorker();
    QObject::connect(m_worker, SIGNAL(resultReady(int, QString, int, quint32, QString)), this, SLOT(onResultReady(int, QString, int, quint32, QString)), Qt::QueuedConnection);
    m_thread = new QThread(this);
    m_worker->moveToThread(m_thread);
    m_thread->start();
}

void HttpsRequestWorkerThread::start(){
    m_thread->start();
}

void HttpsRequestWorkerThread::stop(){
    m_thread->quit();
}

void HttpsRequestWorkerThread::get(QObject* obj){
    QMetaObject::invokeMethod(m_worker, "get", Qt::QueuedConnection, Q_ARG(QObject*, obj));
}

void HttpsRequestWorkerThread::post(QObject* obj){
    QMetaObject::invokeMethod(m_worker, "post", Qt::QueuedConnection, Q_ARG(QObject*, obj));
}

void HttpsRequestWorkerThread::put(QObject* obj){
    QMetaObject::invokeMethod(m_worker, "put", Qt::QueuedConnection, Q_ARG(QObject*, obj));
}

void HttpsRequestWorkerThread::del(QObject* obj){
    QMetaObject::invokeMethod(m_worker, "del", Qt::QueuedConnection, Q_ARG(QObject*, obj));
}

void HttpsRequestWorkerThread::propfind(QObject* obj){
    QMetaObject::invokeMethod(m_worker, "propfind", Qt::QueuedConnection, Q_ARG(QObject*, obj));
}

void HttpsRequestWorkerThread::onResultReady(int reqId, QString result, int errorCode, quint32 statusCode, QString uniqueRequestId){
    emit resultReady(reqId, result, errorCode, statusCode, uniqueRequestId);
}

HttpsRequestWorkerThread::~HttpsRequestWorkerThread(){
    m_thread->quit();
    if(!m_thread->wait(200)){
        m_thread->terminate();
    }
    delete m_thread;
}
