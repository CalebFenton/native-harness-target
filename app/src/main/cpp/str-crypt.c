#include <string.h>
#include <jni.h>

#include <stdio.h>>

void xor_crypt(char *key, char *str, int str_len);

JNIEXPORT jstring JNICALL
Java_org_cf_nativeharness_Cryptor_decryptString( JNIEnv* env,
                                                 jobject thiz,
                                                 jstring encryptedString )
{
    char *str = (*env)->GetStringUTFChars(env, encryptedString, NULL);
    xor_crypt("{$}", str, strlen(str));

    return (*env)->NewStringUTF(env, str);
}

void xor_crypt(char *key, char *str, int str_len)
{
    int i;
    int key_len = strlen(key);
    for(i = 0; i < str_len; i++) {
        str[i] = str[i] ^ key[i % key_len];
    }
}
