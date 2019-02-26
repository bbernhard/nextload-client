#ifndef LISTFOLDERCONTENTSREQUEST_H
#define LISTFOLDERCONTENTSREQUEST_H

#include "sslrequest.h"
#include "basicrequest.h"

class ListFolderContentsRequest : public BasicRequest {
    Q_OBJECT
public:
    ListFolderContentsRequest();
    Q_INVOKABLE void setFolderName(const QString& folderName);
    ~ListFolderContentsRequest();
};

#endif // LISTFOLDERCONTENTSREQUEST_H
