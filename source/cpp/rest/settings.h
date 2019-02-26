#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QQmlEngine>

class ConnectionSettings : public QObject
{
    Q_OBJECT
public:
    //singleton type provider function for Qt Quick
    static QObject* connectionSettingsProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static ConnectionSettings* instance();
    Q_INVOKABLE void setBaseUrl(const QString& baseUrl);
    Q_INVOKABLE QString getBaseUrl() const;
    void setVerifySSL(const bool verifySSL);
    bool getVerifySSL() const;
    void setEnforceSecureCiphers(const bool enforceSecureCiphers);
    bool enforceSecureCiphers() const;
private:
    ConnectionSettings() : m_baseUrl(""), m_verifySSL(false),
                            m_enforceSecureCiphers(false) {};


    // copy constructor not implemented
    ConnectionSettings(ConnectionSettings const&); // don't Implement
    void operator=(ConnectionSettings const&); // don't implement

    QString m_baseUrl;
    bool m_verifySSL;
    bool m_enforceSecureCiphers;

};

#endif // SETTINGS_H
