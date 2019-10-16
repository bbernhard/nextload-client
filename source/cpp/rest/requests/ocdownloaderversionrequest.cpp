#include "ocdownloaderversionrequest.h"
#include <QUrlQuery>
#include <QDebug>
#include <QUuid>

OcDownloaderVersionRequest::OcDownloaderVersionRequest()
    : BasicRequest ()
{
    QUrl url(m_baseUrl + "/index.php/apps/ocdownloader/api/version?format=json");
    m_request->setUrl(url);

    m_request->setRawHeader("OCS-APIREQUEST", "true");
    m_request->setRawHeader("Content-Type", "application/x-www-form-urlencoded");
}

OcDownloaderVersionRequest::~OcDownloaderVersionRequest() {
}
