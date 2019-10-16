#ifndef OCDOWNLOADERADDREQUEST_H
#define OCDOWNLOADERADDREQUEST_H

#include "sslrequest.h"
#include "basicrequest.h"

class OcDownloaderAddRequest : public BasicRequest {
    Q_OBJECT
public:
    OcDownloaderAddRequest();
    Q_INVOKABLE void setDownloadUrl(const QString& downloadUrl);
    ~OcDownloaderAddRequest();
};

#endif // OCDOWNLOADERADDREQUEST_H
