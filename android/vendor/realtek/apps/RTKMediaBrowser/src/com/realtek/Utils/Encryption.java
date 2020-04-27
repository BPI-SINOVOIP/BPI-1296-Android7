package com.realtek.Utils;

import android.util.Base64;
import android.util.Log;

import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.PBEKeySpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.*;
import java.security.cert.CertificateException;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.KeySpec;
import java.util.Enumeration;
import java.util.Random;

/**
 * Created by archerho on 2016/5/13.
 */
public class Encryption {

    private static final String TAG = "Encryption";
    private static final boolean DEBUG = false;
    private static Encryption mInst;
    private final String KEY_NAME = "secretKey";
    private String mSalt = "2380209820";
    private final String CHARSET_NAME = "UTF8";
    private final int ITERATION_COUNT =1000;
    private final int KEY_LENGTH = 128;
    private final String KEY_ALGORITHM = "AES/CBC/PKCS7Padding";
    private final String SECRET_KEY_TYPE = "PBKDF2WithHmacSHA1";
    private static final String HASH_ALGORITHM = "SHA-256";
    private SecureRandom mSr;

    public static Encryption getInstance(){
        if(mInst==null)
            mInst = new Encryption();
        return mInst;
    }

    private Encryption(){
        mSr = new SecureRandom();
    }

    private SecretKey getSecretKey(char[] key) throws NoSuchAlgorithmException, UnsupportedEncodingException, InvalidKeySpecException {
        SecretKeyFactory factory = SecretKeyFactory.getInstance(SECRET_KEY_TYPE);
        KeySpec spec = new PBEKeySpec(key, mSalt.getBytes(CHARSET_NAME), ITERATION_COUNT, KEY_LENGTH);
        SecretKey tmp = factory.generateSecret(spec);
        return new SecretKeySpec(tmp.getEncoded(), KEY_ALGORITHM);
    }

    /**
     * Generates SHA256 hash of the password which is used as key
     *
     * @param password used to generated key
     * @return SHA256 of the password
     */
    private static SecretKeySpec generateKey(final String password) throws NoSuchAlgorithmException, UnsupportedEncodingException {
        final MessageDigest digest = MessageDigest.getInstance(HASH_ALGORITHM);
        byte[] bytes = password.getBytes("UTF-8");
        digest.update(bytes, 0, bytes.length);
        byte[] key = digest.digest();

//        log("SHA-256 key "+key);

        SecretKeySpec secretKeySpec = new SecretKeySpec(key, "AES");
        return secretKeySpec;
    }

    public String encrypt(String data, String key, String iv) {
        if (data == null) return null;
        SecretKey secretKey = null;
        try {
            final SecretKeySpec keySpec = generateKey(key);
            byte[] cipherText = encrypt(keySpec, Base64.decode(iv, Base64.NO_WRAP), data.getBytes(CHARSET_NAME));
            return Base64.encodeToString(cipherText, Base64.NO_WRAP);
        } catch (GeneralSecurityException e) {
            e.printStackTrace();
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        return null;
    }

    public String decrypt(String data, String key, String iv) {
        if (data == null) return null;
        byte[] dataBytes = Base64.decode(data, Base64.NO_WRAP);
        byte[] ivBytes = Base64.decode(iv, Base64.NO_WRAP);
        final SecretKeySpec keySpec;
        try {
            keySpec = generateKey(key);
            byte[] dataBytesDecrypted = decrypt(keySpec, ivBytes, dataBytes);
            return new String(dataBytesDecrypted, CHARSET_NAME);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        } catch (GeneralSecurityException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * More flexible AES encrypt that doesn't encode
     * @param key AES key typically 128, 192 or 256 bit
     * @param iv Initiation Vector
     * @param message in bytes (assumed it's already been decoded)
     * @return Encrypted cipher text (not encoded)
     * @throws GeneralSecurityException if something goes wrong during encryption
     */
    public byte[] encrypt(final SecretKeySpec key, final byte[] iv, final byte[] message)
            throws GeneralSecurityException {
        final Cipher cipher = Cipher.getInstance(KEY_ALGORITHM);
        IvParameterSpec ivSpec = new IvParameterSpec(iv);
        cipher.init(Cipher.ENCRYPT_MODE, key, ivSpec);
        byte[] cipherText = cipher.doFinal(message);

        return cipherText;
    }


    /**
     * More flexible AES decrypt that doesn't encode
     *
     * @param key AES key typically 128, 192 or 256 bit
     * @param iv Initiation Vector
     * @param decodedCipherText in bytes (assumed it's already been decoded)
     * @return Decrypted message cipher text (not encoded)
     * @throws GeneralSecurityException if something goes wrong during encryption
     */
    public byte[] decrypt(final SecretKeySpec key, final byte[] iv, final byte[] decodedCipherText)
            throws GeneralSecurityException {
        final Cipher cipher = Cipher.getInstance(KEY_ALGORITHM);
        IvParameterSpec ivSpec = new IvParameterSpec(iv);
        cipher.init(Cipher.DECRYPT_MODE, key, ivSpec);
        byte[] decryptedBytes = cipher.doFinal(decodedCipherText);
        return decryptedBytes;
    }

    public byte[] generateRndBytes(int byteCount) {
        byte[] iv = new byte[byteCount];
        if(mSr!=null) mSr.nextBytes(iv);
        return iv;
    }

    private static final String ALLOWED_CHARACTERS ="0123456789qwertyuiopasdfghjklzxcvbnm";

    public String generateRndString(final int sizeOfRandomString)
    {
        if(mSr==null) return "";
        final StringBuilder sb=new StringBuilder(sizeOfRandomString);
        for(int i=0;i<sizeOfRandomString;++i)
            sb.append(ALLOWED_CHARACTERS.charAt(mSr.nextInt(ALLOWED_CHARACTERS.length())));
        return sb.toString();
    }

    private static void log(String s){
        if(DEBUG)
            Log.d(TAG, s);
    }
}
