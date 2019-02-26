#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QImage>

class ImageProcessor : public QObject{
    Q_OBJECT
public:
    ImageProcessor(QObject* parent = 0);
    Q_INVOKABLE void setImage(const QString& path);
    Q_INVOKABLE QString getBase64();
    Q_INVOKABLE QByteArray get();
    Q_INVOKABLE QByteArray compress(const quint16 width = 800, const quint16 height = 600, const quint8 quality = 85, const QString& format = "jpg");
    ~ImageProcessor();
private:
    QImage m_image;
};

class Base64ImageProcessor : public QObject{
    Q_OBJECT
public:
    Base64ImageProcessor(QObject* parent = 0);
    Q_INVOKABLE void setImage(const QString& base64Image);
    Q_INVOKABLE QByteArray compress(const quint16 width = 800, const quint16 height = 600, const quint8 quality = 85, const QString& format = "jpg");
    ~Base64ImageProcessor();
private:
    QImage m_image;
};

#endif // IMAGEPROCESSOR_H
