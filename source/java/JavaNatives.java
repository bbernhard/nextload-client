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
        Store store = new Store(context);
        if(!store.hasKey(SHARED_PREFENCE_NAME)) {
           store.generateSymmetricKey(SHARED_PREFENCE_NAME, null);
        }
        SecretKey key = store.getSymmetricKey(SHARED_PREFENCE_NAME, null);
        Crypto crypto = new Crypto(Options.TRANSFORMATION_SYMMETRIC);
        String decryptedData = crypto.decrypt(str, key);
        return decryptedData;
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

    /*public static byte[] rsaEncrypt(byte[] secret, KeyStore keyStore) throws Exception{
        KeyStore.PrivateKeyEntry privateKeyEntry = (KeyStore.PrivateKeyEntry) keyStore.getEntry(KEY_ALIAS, null);
        // Encrypt the text
        Cipher inputCipher = Cipher.getInstance(RSA_MODE, "AndroidOpenSSL");
        inputCipher.init(Cipher.ENCRYPT_MODE, privateKeyEntry.getCertificate().getPublicKey());

        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        CipherOutputStream cipherOutputStream = new CipherOutputStream(outputStream, inputCipher);
        cipherOutputStream.write(secret);
        cipherOutputStream.close();

        byte[] vals = outputStream.toByteArray();
        return vals;
    }

    public static byte[] rsaDecrypt(byte[] encrypted, KeyStore keyStore) throws Exception {
        KeyStore.PrivateKeyEntry privateKeyEntry = (KeyStore.PrivateKeyEntry) keyStore.getEntry(KEY_ALIAS, null);
        Cipher output = Cipher.getInstance(RSA_MODE, "AndroidOpenSSL");
        output.init(Cipher.DECRYPT_MODE, privateKeyEntry.getPrivateKey());
        CipherInputStream cipherInputStream = new CipherInputStream(
                new ByteArrayInputStream(encrypted), output);
        ArrayList<Byte> values = new ArrayList<>();
        int nextByte;
        while ((nextByte = cipherInputStream.read()) != -1) {
            values.add((byte)nextByte);
        }

        byte[] bytes = new byte[values.size()];
        for(int i = 0; i < bytes.length; i++) {
            bytes[i] = values.get(i).byteValue();
        }
        return bytes;
    }

    public static Key getSecretKey(Context context, KeyStore keyStore) throws Exception{
        SharedPreferences pref = context.getSharedPreferences(SHARED_PREFENCE_NAME, Context.MODE_PRIVATE);
        String enryptedKeyB64 = pref.getString(ENCRYPTED_KEY, null);

        if(enryptedKeyB64 != null){
            byte[] encryptedKey = Base64.decode(enryptedKeyB64, Base64.DEFAULT);
            byte[] key = rsaDecrypt(encryptedKey, keyStore);
            return new SecretKeySpec(key, "AES");
        }
        return null;
    }

    public static String encrypt(Context context, String str) {
        try{
            int currentAPIVersion = Build.VERSION.SDK_INT;

            //when Android version < Android M, access keystore that way
            if(currentAPIVersion < Build.VERSION_CODES.M){
                byte[] input = str.getBytes(Charset.forName("UTF-8"));
                KeyStore keyStore = KeyStore.getInstance("AndroidKeyStore");
                keyStore.load(null);
                Cipher c = Cipher.getInstance(AES_MODE, "BC");
                Key k = getSecretKey(context, keyStore);
                if(k != null){
                    c.init(Cipher.ENCRYPT_MODE, k);
                    byte[] encodedBytes = c.doFinal(input);
                    String encryptedBase64Encoded =  Base64.encodeToString(encodedBytes, Base64.DEFAULT);
                    return encryptedBase64Encoded;
                }
                else{
                    Log.e(TAG, "Encrypting key: secret key is null");
                }
            }
            else{ //and when Android version >= Android M, access keystore that way
                byte[] input = str.getBytes(Charset.forName("UTF-8"));
                KeyStore keyStore = KeyStore.getInstance("AndroidKeyStore");
                keyStore.load(null);
                KeyStore.PrivateKeyEntry privateKeyEntry = (KeyStore.PrivateKeyEntry) keyStore.getEntry(JavaNatives.KEY_ALIAS, null);
                PublicKey publicKey = (PublicKey) privateKeyEntry.getCertificate().getPublicKey();
                Cipher c = Cipher.getInstance("RSA/ECB/OAEPWithSHA-256AndMGF1Padding");

                c.init(Cipher.ENCRYPT_MODE, publicKey);
                byte[] encodedBytes = c.doFinal(input);

                String encryptedBase64Encoded = Base64.encodeToString(encodedBytes, Base64.DEFAULT);
                return encryptedBase64Encoded;
            }
        }
        catch(Exception e){
            Log.e(TAG, "Encrypting key: Couldn't encrypt key", e);
        }
        return "";
    }

    public static String decrypt(Context context, String str) {
        try{
            int currentAPIVersion = Build.VERSION.SDK_INT;

            //when Android version < Android M, access keystore that way
            if(currentAPIVersion < Build.VERSION_CODES.M){
                byte[] encrypted = Base64.decode(str, Base64.DEFAULT);

                KeyStore keyStore = KeyStore.getInstance("AndroidKeyStore");
                keyStore.load(null);
                Cipher c = Cipher.getInstance(AES_MODE, "BC");
                Key k = getSecretKey(context, keyStore);
                if(k != null){
                    c.init(Cipher.DECRYPT_MODE, k);
                    byte[] decodedBytes = c.doFinal(encrypted);
                    String decodedStr = new String(decodedBytes, "UTF-8");
                    return decodedStr;
                }
                else{
                    Log.e(TAG, "Decrypting key: secret key is null ");
                }
            }
            else{ //and when Android version >= Android M, access keystore that way
                byte[] encrypted = Base64.decode(str, Base64.DEFAULT);

                KeyStore keyStore = KeyStore.getInstance("AndroidKeyStore");
                keyStore.load(null);

                KeyStore.PrivateKeyEntry privateKeyEntry = (KeyStore.PrivateKeyEntry) keyStore.getEntry(JavaNatives.KEY_ALIAS, null);
                PrivateKey privateKey = (PrivateKey) privateKeyEntry.getPrivateKey();

                Cipher c =  Cipher.getInstance("RSA/ECB/OAEPWithSHA-256AndMGF1Padding");
                c.init(Cipher.DECRYPT_MODE, privateKey);
                byte[] decodedBytes = c.doFinal(encrypted);
                String decodedStr = new String(decodedBytes, "UTF-8");
                return decodedStr;
            }
        }
        catch(Exception e){
            Log.e(TAG, "Decrypting key: Couldn't decrypt key", e);
        }
        return "";
    }*/
}
