#ifndef __UPLOAD_REQUEST_H__
#define __UPLOAD_REQUEST_H__

#include "sslrequest.h"
#include "basicrequest.h"

class UploadRequest : public BasicRequest {
    Q_OBJECT
public:
    UploadRequest();
    Q_INVOKABLE void set(const QByteArray& fileData, const QString& name);
    ~UploadRequest();
};

#endif //__UPLOAD_REQUEST_H__
