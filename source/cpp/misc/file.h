#ifndef _FILE_H
#define _FILE_H

#include<QObject>
#include<QString>
#include<QFile>
#include<QTextStream>
#include<QUrl>
#include<QThread>

class FileType : public QObject{
    Q_OBJECT
    Q_ENUMS(EnFileType)
public:
    enum EnFileType{
        UNKNOWN,
        IMAGE,
        GIF
    };
    Q_ENUMS(EnFileType)
};

class FilePaths : public QObject{
    Q_OBJECT
public:
    FilePaths(QObject *parent = 0);
    Q_INVOKABLE QString picturesLocation() const;
    ~FilePaths();
};

class Dir : public QObject{
    Q_OBJECT
public:
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
    Dir(QObject *parent = 0);
    void setPath(const QString& path);
    QString getPath() const;
    Q_INVOKABLE void create();
    ~Dir();
private:
    QString m_path;
signals:
    void pathChanged();
};

class File : public QObject{
Q_OBJECT
public:
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString destination READ getDestination WRITE setDestination NOTIFY destinationChanged)
    File(QObject *parent = 0);

    Q_INVOKABLE QString read(const bool encodeBase64 = true);
    Q_INVOKABLE void read(const bool encodeBase64, const bool getMimeType);
    Q_INVOKABLE bool write(const QString& data, const bool base64Encoded = false);
    Q_INVOKABLE int fileType() const;
    Q_INVOKABLE QString fileName() const;
    Q_INVOKABLE bool remove();
    void setDestination(const QString& destination);
    QString getDestination() const;

    QString source() const;

public slots:
    void setSource(const QString &source);

signals:
    void sourceChanged(const QString& source);
    void destinationChanged(const QString& destination);
    void error(const QString& msg);
    void fileRead(QString fileName, QString content, int contentType);

private:
    QString m_source;
    QString m_destination;
    QByteArray m_content;
};


class FileWorker : public QObject{
Q_OBJECT
public:
    FileWorker(QObject* parent = 0);
    Q_INVOKABLE void read(const QString &filename, const bool encodeBase64);
    ~FileWorker();
private:
    File* m_file;
    QThread* m_thread;
private slots:
    void onFileRead(QString filename, QString content, int mimeType);
signals:
    void fileRead(QString filename,QString content, int mimeType);
};

#endif /*_FILE_H*/
