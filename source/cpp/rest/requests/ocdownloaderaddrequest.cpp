#include "ocdownloaderaddrequest.h"
#include <QUrlQuery>
#include <QDebug>
#include <QUuid>

OcDownloaderAddRequest::OcDownloaderAddRequest()
    : BasicRequest ()
{
}

void OcDownloaderAddRequest::setDownloadUrl(const QString &downloadUrl) {
    QUrl url(m_baseUrl + "/index.php/apps/ocdownloader/api/add?format=json");
    m_request->setUrl(url);

    m_request->setRawHeader("OCS-APIREQUEST", "true");
    m_request->setRawHeader("Content-Type", "application/x-www-form-urlencoded");


    QUrlQuery postData;
    postData.addQueryItem("URL", downloadUrl);
    setData(postData.toString(QUrl::FullyEncoded).toUtf8());
}

OcDownloaderAddRequest::~OcDownloaderAddRequest() {
}
