#include "listfoldercontentsrequest.h"
#include <QUrlQuery>
#include <QUuid>

ListFolderContentsRequest::ListFolderContentsRequest()
    : BasicRequest ()
{
}

void ListFolderContentsRequest::setFolderName(const QString &folderName) {
    QUrl url(m_baseUrl + "/remote.php/webdav/" + folderName);
    m_request->setUrl(url);
}

ListFolderContentsRequest::~ListFolderContentsRequest() {
}
