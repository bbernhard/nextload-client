#ifndef __MAIN__H__
#define __MAIN__H__

#include "executiontargetdef.h"
#include "rest/httpsrequestworker.h"
#include "misc/file.h"
#include "misc/imageprocessor.h"
#include "rest/requests/upload.h"
#include "rest/requests/listfoldercontentsrequest.h"
#include "rest/requests/ocdownloaderversionrequest.h"
#include "rest/requests/ocdownloaderaddrequest.h"
#include "misc/base64imageprovider.h"

#ifdef Q_OS_IOS
#include "misc/ios/keychain.h"
#endif

#ifdef Q_OS_WIN32
#include "misc/windows/keychain.h"
#endif

#ifdef Q_OS_LINUX
#include "misc/linux/keychain.h"
#endif

#ifdef Q_OS_OSX
#include "misc/osx/keychain.h"
#endif

#ifdef Q_OS_ANDROID
#include "misc/android/keychain.h"
#endif

#endif /*#ifndef __MAIN__H__*/
