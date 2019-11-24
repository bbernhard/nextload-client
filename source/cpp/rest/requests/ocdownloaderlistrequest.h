#ifndef OCDOWNLOADERLISTREQUEST_H
#define OCDOWNLOADERLISTREQUEST_H


#include "sslrequest.h"
#include "basicrequest.h"

class OcDownloaderListRequest : public BasicRequest {
    Q_OBJECT
public:
    OcDownloaderListRequest();
    ~OcDownloaderListRequest();
};

#endif // OCDOWNLOADERLISTREQUEST_H
