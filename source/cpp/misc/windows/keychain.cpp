#include <QString>
#include <QImage>
#include <QQmlEngine>
#include "keychain.h"

namespace windows {
    KeyChain::KeyChain(QObject *parent)
        : KeyChainInterface(parent),
          m_token("")
    {
    }

    void KeyChain::setToken(const QString &token){
        set("token", token);
        m_token = token;
    }

    QString KeyChain::getToken() const{
        if(m_token != "")
            return m_token;
        return get("token");
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

