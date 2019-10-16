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
    setData(fileData);
}

UploadRequest::~UploadRequest() {
}
