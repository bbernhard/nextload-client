#ifndef LINUXKEYCHAIN_H
#define LINUXKEYCHAIN_H

#include <QString>
#include <QImage>
#include <QQmlEngine>
#include <QSettings>
#include "../keychaininterface.h"

namespace linux1 {
    class KeyChain : public KeyChainInterface{
        Q_OBJECT
    public:
        KeyChain(QObject* parent = 0);
        Q_INVOKABLE void setToken(const QString& jwt);
        Q_INVOKABLE QString getToken() const;
        ~KeyChain();
    private:
        QSettings m_settings;
        QString m_token;

        void set(const QString& key, const QString& value);
        QString get(const QString& key) const;

    };
}


#endif // LINUXKEYCHAIN_H
