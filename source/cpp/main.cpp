#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "main.h"
#include "executiontarget.h"
#include "rest/settings.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif


int main(int argc, char *argv[])
{
    const QString applicationName = "nextloadclient";
    const QString applicationVersion = "1.0.0";
    const QString g_uri = "com." + applicationName + "." + applicationName;

    //High dpi scaling doesn't work, see: https://bugreports.qt.io/browse/QTBUG-53247
    //(issue still exists, even in Qt 5.12.3)
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    ConnectionSettings::instance()->setVerifySSL(true);
    ConnectionSettings::instance()->setEnforceSecureCiphers(true);

    app.setApplicationName(applicationName);
    app.setApplicationVersion(applicationVersion);

    QString executionTarget = "";
    if(EXECUTIONTARGET == ExcecutionTarget::LOCAL){
        app.setOrganizationName(applicationName+"-local");
        executionTarget = "Local";
    }
    else if(EXECUTIONTARGET == ExcecutionTarget::TEST){
        app.setOrganizationName(applicationName+"-test");
        executionTarget = "Test";
    }
    else if(EXECUTIONTARGET == ExcecutionTarget::PRODUCTION){
        app.setOrganizationName(applicationName);
        executionTarget = "Production";
    }
    app.setOrganizationDomain(g_uri);

    QQmlApplicationEngine engine;

    qmlRegisterType<HttpsRequestWorker>(g_uri.toStdString().c_str(), 1, 0, "HttpsRequestWorker");
    qmlRegisterType<HttpsRequestWorkerThread>(g_uri.toStdString().c_str(), 1, 0, "HttpsRequestWorkerThread");
    qmlRegisterType<ImageProcessor>(g_uri.toStdString().c_str(), 1, 0, "ImageProcessor");
    qmlRegisterType<Base64ImageProcessor>(g_uri.toStdString().c_str(), 1, 0, "Base64ImageProcessor");
    qmlRegisterType<File>(g_uri.toStdString().c_str(), 1, 0, "File");
    qmlRegisterType<FileWorker>(g_uri.toStdString().c_str(), 1, 0, "FileWorker");
    qmlRegisterType<Dir>(g_uri.toStdString().c_str(), 1, 0, "Dir");
    qmlRegisterType<FilePaths>(g_uri.toStdString().c_str(), 1, 0, "FilePaths");
    qmlRegisterType<UploadRequest>(g_uri.toStdString().c_str(), 1, 0, "UploadRequest");
    qmlRegisterType<ListFolderContentsRequest>(g_uri.toStdString().c_str(), 1, 0, "ListFolderContentsRequest");

    //ocDownloader
    qmlRegisterType<OcDownloaderVersionRequest>(g_uri.toStdString().c_str(), 1, 0, "OcDownloaderVersionRequest");
    qmlRegisterType<OcDownloaderAddRequest>(g_uri.toStdString().c_str(), 1, 0, "OcDownloaderAddRequest");
    qmlRegisterType<OcDownloaderListRequest>(g_uri.toStdString().c_str(), 1, 0, "OcDownloaderListRequest");

    //register the different implementations, depending on the platform
#if defined(Q_OS_IOS)
    qmlRegisterType<ios::KeyChain>(g_uri.toStdString().c_str(), 1, 0, "Keychain");
#elif defined(Q_OS_OSX)
    qmlRegisterType<osx::KeyChain>(g_uri.toStdString().c_str(), 1, 0, "Keychain");
#elif defined(Q_OS_WIN)
    qmlRegisterType<windows::KeyChain>(g_uri.toStdString().c_str(), 1, 0, "Keychain");
#elif defined(Q_OS_ANDROID)
    qmlRegisterType<android::KeyChain>(g_uri.toStdString().c_str(), 1, 0, "Keychain");
#elif defined(Q_OS_LINUX)
    qmlRegisterType<linux1::KeyChain>(g_uri.toStdString().c_str(), 1, 0, "Keychain");
#endif


    qmlRegisterSingletonType<ConnectionSettings>(g_uri.toStdString().c_str(), 1, 0, "ConnectionSettings",
                                                                           ConnectionSettings::connectionSettingsProvider);

    engine.addImportPath(QStringLiteral("qrc:/"));
    engine.load(QUrl(QLatin1String("qrc:/source/qml/main.qml")));

    Base64ImageProvider* base64ImageProvider = new Base64ImageProvider();
    engine.addImageProvider(QLatin1String("base64"), base64ImageProvider);
    engine.rootContext()->setContextProperty("base64ImageProvider", base64ImageProvider);

    //the splashscreen was defined as "sticky" with the following line
    //meta-data android:name="android.app.splash_screen_sticky" android:value="true"
    //in the AndroidManifest. After the heavy lifting is done (i.e engine is loaded)
    //we can hide the splashscreen. This removes the white flickering that otherwise would
    //occur during the engine loads its data
#ifdef Q_OS_ANDROID
    QtAndroid::hideSplashScreen();
#endif

    return app.exec();
}
