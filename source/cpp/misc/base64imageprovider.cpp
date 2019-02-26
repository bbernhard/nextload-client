#include "base64imageprovider.h"

Base64ImageProvider::Base64ImageProvider(QObject* parent)
  :  QQuickImageProvider(QQuickImageProvider::Image),
     QObject(parent)
{
}

Base64ImageProvider::~Base64ImageProvider()
{
}

QQmlImageProviderBase::Flags Base64ImageProvider::flags() const
{
    return QQmlImageProviderBase::ForceAsynchronousImageLoading;
}

QImage Base64ImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
    QImage img = QImage::fromData(QByteArray::fromBase64(id.toUtf8()));

    if (size)
        *size = QSize(img.width(), img.height());

    if (requestedSize.isValid() && (img.size() != requestedSize))
        img = img.scaled(requestedSize, Qt::KeepAspectRatio);
    return img;
}
