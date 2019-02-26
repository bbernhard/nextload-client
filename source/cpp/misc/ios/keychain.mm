#include "keychain.h"
#include "keychainimpl.h"
#include <QQmlEngine>

namespace ios{

    #define SERVICE_NAME @"gympulsr"
    #define GROUP_NAME @"com.gympulsr.gympulsr.com.apps.shared" //GROUP NAME should start with application identifier.

    Keychain* keychain =[[Keychain alloc] initWithService:SERVICE_NAME withGroup:nil];

    KeyChain::KeyChain(QObject *parent)
        : KeyChainInterface(parent),
          m_jwt("")
    {
    }

    bool KeyChain::add(const QString &key, const QString &value){
        bool success = false;
        NSString* keyStr = key.toNSString();
        NSData * valueStr = [value.toNSString() dataUsingEncoding:NSUTF8StringEncoding];

        if([keychain insert:keyStr :valueStr])
         {
             NSLog(@"Successfully added data");
             success = true;
         }
         else{
          NSLog(@"Failed to  add data");
          success = false;
        }
        return success;
    }

    QString KeyChain::get(const QString &key) const{
        QString data = "";
        NSString* keyStr = key.toNSString();
        NSData* dataStr = [keychain find:keyStr];

        NSLog(@"Reading keychain data");

        if(dataStr == nil)
        {
            NSLog(@"Keychain data not found");
        }
        else
        {
            //NSLog(@"Data is =%@",[[NSString alloc] initWithData:dataStr encoding:NSUTF8StringEncoding]);
            NSString* res = [[NSString alloc] initWithData:dataStr encoding:NSUTF8StringEncoding];
            data = QString::fromNSString(res);
        }

        return data;
    }

    bool KeyChain::remove(const QString& key){
        bool success = false;
        NSString *keyStr = key.toNSString();
        if([keychain remove:keyStr])
        {
            NSLog(@"Successfully removed data");
            success = true;
        }
        else
        {
           NSLog(@"Unable to remove data");
           success = false;
        }

        return success;
    }

    bool KeyChain::update(const QString &key, const QString &value){
        NSString* keyStr = key.toNSString();
        NSData* valueStr = [value.toNSString() dataUsingEncoding:NSUTF8StringEncoding];

        if([keychain update:keyStr :valueStr])
        {
            NSLog(@"Successfully updated data");
        }
        else{
          NSLog(@"Failed to  add data");
        }
    }

    bool KeyChain::addOrUpdate(const QString& key, const QString& value){
        if(contains(key)){
            return update(key, value);
        }
        return add(key, value);
    }

    bool KeyChain::contains(const QString& key) const{
        NSString* keyStr = key.toNSString();
        NSData* dataStr = [keychain find:keyStr];
        if(dataStr == nil)
        {
            return false;
        }
        return true;
    }

    void KeyChain::setJwt(const QString &jwt){
        if(addOrUpdate("jwt", jwt)) m_jwt = jwt;
        emit jwtChanged();
    }

    QString KeyChain::getJwt() const{
        if(m_jwt == "")
            return get("jwt");
        return m_jwt;
    }

    KeyChain::~KeyChain(){
    }

}
