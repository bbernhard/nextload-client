#include "file.h"
#include <QDebug>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QStandardPaths>
#include <QDir>

File::File(QObject *parent) :
    QObject(parent),
    m_source(""),
    m_destination("")
{

}

QString File::source() const{
    return m_source;
}

QString File::getDestination() const{
    return m_destination;
}

void File::setDestination(const QString& destination){
    m_destination = destination;
}

void File::setSource(const QString& source){
    QUrl fileUrl(source);

    //convert to local file string, if not already
    if(fileUrl.isLocalFile()) m_source = fileUrl.toLocalFile();
    else m_source = source;
}

QString File::read(const bool encodeBase64){
    QByteArray base64EncodedContent;
    if (m_source.isEmpty()){
        emit error("source is empty");
        qDebug() << "Is empty";
        return QString();
    }

    QFile file(m_source);
    if(!file.open(QIODevice::ReadOnly)){
        emit error("Unable to open the file");
        qDebug() << "error";
        return QString();
    }
    m_content = file.readAll();
    file.close();

    if(encodeBase64){
        base64EncodedContent = QByteArray(m_content.toBase64());
        return QString::fromUtf8(base64EncodedContent.data());
    }
    return m_content;
}

void File::read(const bool encodeBase64, const bool getMimeType){
    QByteArray base64EncodedContent;
    int fType = FileType::UNKNOWN;

    if (m_source.isEmpty()){
        qDebug() << "source is empty";
        return;
    }

    QFile file(m_source);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "unable to open file";
        return;
    }
    m_content = file.readAll();
    file.close();

    if(getMimeType)
        fType = fileType();

    if(encodeBase64){
        base64EncodedContent = QByteArray(m_content.toBase64());
        emit fileRead(fileName(), QString::fromUtf8(base64EncodedContent.data()), fType);
        return;
    }

    emit fileRead(fileName(), m_content, fType);
}

bool File::remove(){
    if(m_source != "") return QFile::remove(m_source);
    return false;
}

bool File::write(const QString& data, const bool base64Encoded){
    bool success = false;
    if (m_destination.isEmpty()) return false;

    QFile file(m_destination);
    if (file.open(QIODevice::WriteOnly)) {
        if(base64Encoded)
            success = file.write(QByteArray::fromBase64(data.toUtf8()));
        else
            success = file.write(data.toUtf8());
    }
    file.close();

    return success;
}

int File::fileType() const{
    QMimeDatabase mimeDatabase;
    QMimeType mimeType;
    if(m_content.size() == 0)
        return FileType::UNKNOWN;

    mimeType = mimeDatabase.mimeTypeForData(m_content);
    if(mimeType.inherits("image/jpg") || mimeType.inherits("image/jpg") ||
         mimeType.inherits("image/jpeg") || mimeType.inherits("image/png")){
        return FileType::IMAGE;
    }
    if(mimeType.inherits("image/gif"))
        return FileType::GIF;

    return FileType::UNKNOWN;
}

QString File::fileName() const{
    return QFileInfo(m_source).fileName();
}


FileWorker::FileWorker(QObject *parent)
    : QObject(parent)
{
    m_file = new File();
    QObject::connect(m_file, SIGNAL(fileRead(QString, QString, int)), this, SLOT(onFileRead(QString, QString, int)), Qt::QueuedConnection);
    m_thread = new QThread(this);
    m_file->moveToThread(m_thread);
    m_thread->start();
}

void FileWorker::read(const QString &filename, const bool encodeBase64){
    bool getMimeType = true;
    QMetaObject::invokeMethod(m_file, "setSource", Qt::QueuedConnection, Q_ARG(QString, filename));
    QMetaObject::invokeMethod(m_file, "read", Qt::QueuedConnection, Q_ARG(bool, encodeBase64),
                                                                       Q_ARG(bool, getMimeType));
}

void FileWorker::onFileRead(QString filename, QString content, int mimeType){
    emit fileRead(filename, content, mimeType);
}

FileWorker::~FileWorker(){
    m_thread->quit();
    if(!m_thread->wait(200)){
        m_thread->terminate();
    }
    delete m_file;
    delete m_thread;
}


Dir::Dir(QObject *parent)
    : QObject(parent),
      m_path("")
{
}

void Dir::setPath(const QString& path){
    QUrl fileUrl(path);

    //convert to local file string, if not already
    if(fileUrl.isLocalFile()) m_path = fileUrl.toLocalFile();
    else m_path = path;
}

QString Dir::getPath() const{
    return m_path;
}

void Dir::create(){
    QDir dir(m_path);
    if(!dir.exists()){
        dir.mkdir(m_path);
    }
}

Dir::~Dir(){
}

FilePaths::FilePaths(QObject *parent)
    : QObject(parent)
{
}

QString FilePaths::picturesLocation() const{
    return QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/gympulsr/";
}


FilePaths::~FilePaths(){
}
