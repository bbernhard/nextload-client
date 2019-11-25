package com.liftingcoder.nextload;
import org.qtproject.qt5.android.QtNative;
import android.content.Intent;
import android.util.Log;

//for Android keystore
import javax.crypto.Cipher;
import android.util.Base64;
import android.content.Context;
import android.content.SharedPreferences;
import javax.crypto.spec.SecretKeySpec;
import android.security.keystore.KeyProtection;
import java.util.ArrayList;
import java.io.ByteArrayInputStream;
import javax.crypto.CipherInputStream;
import java.security.KeyStore;
import android.util.Log;
import javax.crypto.CipherOutputStream;
import java.security.Key;
import java.security.KeyPairGenerator;
import android.security.keystore.KeyProperties;
import android.security.KeyPairGeneratorSpec;
import javax.security.auth.x500.X500Principal;
import java.math.BigInteger;
import java.util.Calendar;
import java.security.SecureRandom;
import java.io.ByteArrayOutputStream;
import java.nio.charset.Charset;
import android.os.Build;

//for android keystore (Android >= M)
import android.security.KeyPairGeneratorSpec;
import javax.crypto.spec.GCMParameterSpec;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.SecretKey;
import java.security.PublicKey;
import java.security.PrivateKey;


//for hardware key checking
import android.view.KeyCharacterMap;
import android.view.KeyEvent;

//
import android.content.pm.PackageManager;

import com.yakivmospan.scytale.Crypto;
import com.yakivmospan.scytale.Store;
import com.yakivmospan.scytale.Options;

class JavaNatives{
    public static native void sendJwt(String jwt);
    private static final String RSA_MODE = "RSA/ECB/PKCS1Padding";
    private static final String AES_MODE = "AES/ECB/PKCS7Padding";
    private static final String TAG = "JavaNatives";
    public static final String ENCRYPTED_KEY = "encryption_key";
    public static final String SHARED_PREFENCE_NAME = "imagemonkey_credentials";
    public static final String KEY_ALIAS = "key";


    public static boolean hasHardwareBackButton(){
        boolean hasBackKey = KeyCharacterMap.deviceHasKey(KeyEvent.KEYCODE_BACK);
        return hasBackKey;
    }

    public static String decrypt(Context context, String str) {
        if(str != "" && str.length() != 0) {
            Store store = new Store(context);
            if(!store.hasKey(SHARED_PREFENCE_NAME)) {
                store.generateSymmetricKey(SHARED_PREFENCE_NAME, null);
            }
            SecretKey key = store.getSymmetricKey(SHARED_PREFENCE_NAME, null);
            Crypto crypto = new Crypto(Options.TRANSFORMATION_SYMMETRIC);
            String decryptedData = crypto.decrypt(str, key);

            return decryptedData;
        }
        return "";
    }

    public static String encrypt(Context context, String str) {
        Store store = new Store(context);
        if(!store.hasKey(SHARED_PREFENCE_NAME)) {
           store.generateSymmetricKey(SHARED_PREFENCE_NAME, null);
        }
        SecretKey key = store.getSymmetricKey(SHARED_PREFENCE_NAME, null);
        Crypto crypto = new Crypto(Options.TRANSFORMATION_SYMMETRIC);
        String encryptedData = crypto.encrypt(str, key);
        return encryptedData;
    }
}
