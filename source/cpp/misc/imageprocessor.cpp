#include "imageprocessor.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickImageProvider>
#include <QBuffer>
#include <QDebug>

ImageProcessor::ImageProcessor(QObject* parent)
    : QObject(parent)
{
}

void ImageProcessor::setImage(const QString& path){
    QUrl imageUrl(path);
    QQmlEngine* engine = QQmlEngine::contextForObject(this)->engine();
    QQmlImageProviderBase* imageProviderBase = engine->imageProvider(
                imageUrl.host());
    QQuickImageProvider* imageProvider = static_cast<QQuickImageProvider*>
            (imageProviderBase);

    QSize imageSize;
    QString imageId = imageUrl.path().remove(0,1);
    m_image = imageProvider->requestImage(imageId, &imageSize, imageSize);
}

QString ImageProcessor::getBase64(){
    QByteArray base64Transformed;
    QBuffer buffer(&base64Transformed);
    buffer.open(QIODevice::WriteOnly);
    m_image.save(&buffer, "PNG"); // writes image into buffer in PNG format
    return base64Transformed.toBase64();
}

QByteArray ImageProcessor::get(){
    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    m_image.save(&buffer, "PNG"); // writes image into buffer in PNG format

    return arr;
}

QByteArray ImageProcessor::compress(const quint16 width, const quint16 height, const quint8 quality,
                                          const QString& format){
    QImage scaledImage;
    QByteArray buf;
    QBuffer buffer(&buf);
    QSize maxSize(width, height);

    //currently we allow a maximum size
    //if image is larger, it get's scaled
    if((m_image.size().width() > maxSize.width()) && (m_image.size().height() > maxSize.height())){
        scaledImage = m_image.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    else{
        scaledImage = m_image;
    }


    buffer.open(QIODevice::WriteOnly);

    // writes image into buffer in jpg format
    // we use jpg here, as PNG is lossless and we need
    // to compress it a little bit to save space and reduce the size
    scaledImage.save(&buffer, format.toStdString().c_str(), quality);
    return buf;

}

ImageProcessor::~ImageProcessor(){
}


Base64ImageProcessor::Base64ImageProcessor(QObject* parent)
    : QObject(parent)
{
}

void Base64ImageProcessor::setImage(const QString& base64Image){
    m_image = QImage::fromData(QByteArray::fromBase64(base64Image.toUtf8()));
}

QByteArray Base64ImageProcessor::compress(const quint16 width, const quint16 height, const quint8 quality,
                                          const QString& format){
    QImage scaledImage;
    QByteArray buf;
    QBuffer buffer(&buf);
    QSize maxSize(width, height);

    //currently we allow a maximum size
    //if image is larger, it get's scaled
    if((m_image.size().width() > maxSize.width()) && (m_image.size().height() > maxSize.height())){
        scaledImage = m_image.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    else{
        scaledImage = m_image;
    }


    buffer.open(QIODevice::WriteOnly);

    // writes image into buffer in jpg format
    // we use jpg here, as PNG is lossless and we need
    // to compress it a little bit to save space and reduce the size
    scaledImage.save(&buffer, format.toStdString().c_str(), quality);
    return buf.toBase64();

}

Base64ImageProcessor::~Base64ImageProcessor(){
}
