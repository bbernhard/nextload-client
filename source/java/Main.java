package com.liftingcoder.nextload;

import android.content.Context;
import android.os.Vibrator;
import android.app.Activity;
import android.os.Bundle;
import com.liftingcoder.nextload.R;
import android.util.Log;

import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;

//imports for android keystore
import javax.crypto.Cipher;
import android.util.Base64;
import android.content.Context;
import javax.crypto.spec.SecretKeySpec;
import android.security.keystore.KeyProtection;
import java.util.ArrayList;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import javax.crypto.CipherInputStream;
import java.security.KeyStore;
import javax.crypto.CipherOutputStream;
import java.security.Key;
import java.security.KeyPairGenerator;
import android.security.keystore.KeyProperties;
import android.security.KeyPairGeneratorSpec;
import javax.security.auth.x500.X500Principal;
import java.math.BigInteger;
import java.util.Calendar;
import java.security.SecureRandom;
import android.os.Build;

//imports for android keystore (Android >= M)
import android.security.keystore.KeyGenParameterSpec;
import javax.crypto.KeyGenerator;
import java.security.KeyPair;
import java.security.PublicKey;

public class Main extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static final String TAG = "MainActivity";
    private KeyStore m_keyStore;


    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /*//initialize Android keystore
        try{
            int currentAPIVersion = Build.VERSION.SDK_INT; //android.os.Build.VERSION.SDK_INT;

            //initialize android keystore that way, when Android version < Android M
            if(currentAPIVersion < Build.VERSION_CODES.M){

                m_keyStore = KeyStore.getInstance("AndroidKeyStore");
                m_keyStore.load(null);

                // Generate the RSA key pairs (if not exists)
                if (!m_keyStore.containsAlias(JavaNatives.KEY_ALIAS)) {
                    // Generate a key pair for encryption
                    Calendar start = Calendar.getInstance();
                    Calendar end = Calendar.getInstance();
                    end.add(Calendar.YEAR, 30);

                    KeyPairGeneratorSpec spec = new KeyPairGeneratorSpec.Builder(this)
                            .setAlias(JavaNatives.KEY_ALIAS)
                            .setSubject(new X500Principal("CN=" + JavaNatives.KEY_ALIAS))
                            .setSerialNumber(BigInteger.TEN)
                            .setStartDate(start.getTime())
                            .setEndDate(end.getTime())
                            .build();
                    KeyPairGenerator kpg = KeyPairGenerator.getInstance(KeyProperties.KEY_ALGORITHM_RSA, "AndroidKeyStore");
                    kpg.initialize(spec);
                    kpg.generateKeyPair();
                }

                //Generate and Store the AES Key (if not present)
                SharedPreferences pref = this.getSharedPreferences(JavaNatives.SHARED_PREFENCE_NAME, Context.MODE_PRIVATE);
                String enryptedKeyB64 = pref.getString(JavaNatives.ENCRYPTED_KEY, null);
                if (enryptedKeyB64 == null) {
                    byte[] key = new byte[16];
                    SecureRandom secureRandom = new SecureRandom();
                    secureRandom.nextBytes(key);

                    byte[] encryptedKey = JavaNatives.rsaEncrypt(key, m_keyStore);
                    enryptedKeyB64 = Base64.encodeToString(encryptedKey, Base64.DEFAULT);
                    SharedPreferences.Editor edit = pref.edit();
                    edit.putString(JavaNatives.ENCRYPTED_KEY, enryptedKeyB64);
                    edit.commit();
                }
            }
            else{
                m_keyStore = KeyStore.getInstance("AndroidKeyStore");
                m_keyStore.load(null);
                if (!m_keyStore.containsAlias(JavaNatives.KEY_ALIAS)) {
                    Log.i(TAG, "generating key");
                    KeyPairGenerator generator = KeyPairGenerator.getInstance(KeyProperties.KEY_ALGORITHM_RSA, "AndroidKeyStore");
                    KeyGenParameterSpec spec = new  KeyGenParameterSpec.Builder(
                                                JavaNatives.KEY_ALIAS,
                                                KeyProperties.PURPOSE_DECRYPT | KeyProperties.PURPOSE_ENCRYPT )
                                                .setDigests(KeyProperties.DIGEST_SHA256, KeyProperties.DIGEST_SHA512)
                                                .setEncryptionPaddings(KeyProperties.ENCRYPTION_PADDING_RSA_OAEP)
                                                .build();
                    generator.initialize(spec);
                    KeyPair keyPair = generator.generateKeyPair();
                }
            }

        }
        catch(Exception e){
            m_keyStore = null;
            Log.e(TAG, "Couldn't initialize keystore");
        }*/
    }

    @Override
    protected void onResume()
    {
        super.onResume();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
    }

    @Override
    protected void onPause()
    {
        super.onPause();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
    }


    public static Main m_istance;
    public Main()
    {
        m_istance = this;
    }
}
