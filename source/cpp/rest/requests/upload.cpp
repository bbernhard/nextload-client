#include "upload.h"
#include <QUrlQuery>
#include <QUuid>

UploadRequest::UploadRequest()
    : BasicRequest ()
{
}

void UploadRequest::set(const QByteArray& fileData, const QString& name) {
    QUrl url(m_baseUrl + "/remote.php/webdav/nextload/" + name);
    m_request->setUrl(url);

    /*QString uuid = QUuid::createUuid().toByteArray();
    QString boundary = "boundary_.oOo._" + uuid.mid(1,36).toUpper();

    QByteArray data(QString("--" + boundary + "\r\n").toUtf8());
    data.append("Content-Disposition: form-data; name=\"task\"\r\n\r\n");
    data.append((name + "\r\n"));
    data.append("--" + boundary + "\r\n"); //according to rfc 1867
    data.append("Content-Disposition: form-data; name=\"task\"; filename=\"file.jpg\"\r\n");
    data.append("Content-Type: image/jpeg\r\n\r\n"); //data type

    data.append(fileData);
    data.append("\r\n");
    data.append("--" + boundary + "--\r\n"); //closing boundary according to rfc 1867*/

    setData(fileData);

    //m_request->setRawHeader(QString("Content-Type").toUtf8(), QString("multipart/form-data; boundary=" + boundary).toUtf8());
    //m_request->setRawHeader(QString("Content-Length").toUtf8(), QString::number(data.length()).toUtf8());
}

UploadRequest::~UploadRequest() {
}
