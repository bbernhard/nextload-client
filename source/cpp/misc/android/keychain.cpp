#include <QString>
#include <QImage>
#include <QDebug>
#include <QQmlEngine>
#include <QtAndroid>
#include "keychain.h"

namespace android {
    KeyChain::KeyChain(QObject *parent)
        : KeyChainInterface(parent),
          m_token("")
    {
    }

    void KeyChain::setToken(const QString &token){
        QAndroidJniObject activity =  QtAndroid::androidActivity();
        QAndroidJniObject appContext = activity.callObjectMethod("getApplicationContext","()Landroid/content/Context;");
        QAndroidJniObject jwtStr = QAndroidJniObject::fromString(token);
        QAndroidJniObject result = QAndroidJniObject::callStaticObjectMethod("com/liftingcoder/nextload/JavaNatives",
                                                  "encrypt",
                                                  "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;",
                                                  appContext.object<jobject>(), jwtStr.object<jstring>());


        if(result.toString() != "")
            set("token", result.toString()); //save encoded string

        m_token = token; //cache plain jwt in member variable
    }

    QString KeyChain::getToken() const{
        if(m_token != "") //if jwt already cached, return cached jwt
            return m_token;

        //if not cached, get encoded jwt and decode it
        QString jwt = get("token");
        QAndroidJniObject jwtStr = QAndroidJniObject::fromString(jwt);
        QAndroidJniObject result;

        QAndroidJniObject activity =  QtAndroid::androidActivity();
        QAndroidJniObject appContext = activity.callObjectMethod("getApplicationContext","()Landroid/content/Context;");
        result = QAndroidJniObject::callStaticObjectMethod("com/liftingcoder/nextload/JavaNatives",
                                                      "decrypt",
                                                      "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;",
                                                      appContext.object<jobject>(), jwtStr.object<jstring>());

        return result.toString();
    }

    QString KeyChain::get(const QString& key) const{
        return m_settings.value(key, "").toString();
    }

    void KeyChain::set(const QString& key, const QString& value){
        m_settings.setValue(key, value);
    }

    KeyChain::~KeyChain(){
    }
}

