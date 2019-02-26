#include "settings.h"

QObject* ConnectionSettings::connectionSettingsProvider(QQmlEngine *engine, QJSEngine *scriptEngine){
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return ConnectionSettings::instance();
}

ConnectionSettings* ConnectionSettings::instance() {
    static ConnectionSettings* connectionSettings = new ConnectionSettings();
    return connectionSettings;
}

QString ConnectionSettings::getBaseUrl() const{
    return m_baseUrl;
}

void ConnectionSettings::setBaseUrl(const QString& baseUrl){
    m_baseUrl = baseUrl;
}

void ConnectionSettings::setVerifySSL(const bool verifySSL){
    m_verifySSL = verifySSL;
}

bool ConnectionSettings::getVerifySSL() const{
    return m_verifySSL;
}
void ConnectionSettings::setEnforceSecureCiphers(const bool enforceSecureCiphers){
    m_enforceSecureCiphers = true;
}

bool ConnectionSettings::enforceSecureCiphers() const{
    return m_enforceSecureCiphers;
}
