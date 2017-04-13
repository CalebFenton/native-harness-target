package org.cf.nativeharness;

public class Cryptor {

    private static Cryptor instance = null;

    public static Cryptor getInstance() {
        if (instance == null) {
            instance = new Cryptor();
        }

        return instance;
    }

    public native String decryptString(String encryptedString);
}
