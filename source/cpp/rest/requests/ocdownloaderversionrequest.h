#ifndef OCDOWNLOADERVERSIONREQUEST_H
#define OCDOWNLOADERVERSIONREQUEST_H


#include "sslrequest.h"
#include "basicrequest.h"

class OcDownloaderVersionRequest : public BasicRequest {
    Q_OBJECT
public:
    OcDownloaderVersionRequest();
    ~OcDownloaderVersionRequest();
};


#endif // OCDOWNLOADERVERSIONREQUEST_H
