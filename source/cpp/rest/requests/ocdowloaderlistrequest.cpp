#include "ocdownloaderlistrequest.h"
#include <QUrlQuery>
#include <QDebug>
#include <QUuid>

OcDownloaderListRequest::OcDownloaderListRequest()
    : BasicRequest ()
{
    QUrl url(m_baseUrl + "/index.php/apps/ocdownloader/api/queue/get?format=json");
    m_request->setUrl(url);

    m_request->setRawHeader("OCS-APIREQUEST", "true");
    m_request->setRawHeader("Content-Type", "application/x-www-form-urlencoded");
}

OcDownloaderListRequest::~OcDownloaderListRequest() {
}
