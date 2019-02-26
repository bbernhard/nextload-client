#ifndef KEYCHAININTERFACE_H
#define KEYCHAININTERFACE_H

#include <QObject>

class KeyChainInterface : public QObject{
    Q_OBJECT
public:
    KeyChainInterface(QObject* parent = 0)
        : QObject(parent)
    {}
    virtual void setToken(const QString& token) = 0;
    virtual QString getToken() const = 0;
    virtual ~KeyChainInterface() {}
};

#endif // KEYCHAININTERFACE_H
