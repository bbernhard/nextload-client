#ifndef _BASE664IMAGEPROVIDER_H
#define _BASE664IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QObject>

class Base64ImageProvider : public QObject, public QQuickImageProvider{
    Q_OBJECT
public:
    Base64ImageProvider(QObject* parent = 0);
    //Q_INVOKABLE void fromBase64(const QString& base64Str);
    virtual QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize);
    virtual QQmlImageProviderBase::Flags flags() const;
    ~Base64ImageProvider();
private:
    QByteArray m_base64Str;
};

#endif //_BASE664IMAGEPROVIDER_H
