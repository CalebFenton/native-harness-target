#include <string.h>
#include <jni.h>

#include <stdio.h>

void xor_crypt(char *key, char *str, int str_len);

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    return JNI_VERSION_1_4;
}

JNIEXPORT jstring JNICALL
Java_org_cf_nativeharness_Cryptor_decryptString( JNIEnv *env,
                                                 jobject thiz,
                                                 jstring encryptedString )
{
    // Explicitly cast to char * to avoid warning:
    // initializing 'char *' with an expression of type 'const char *'
    // We're going to be changing bytes in xor_crypt
    char *str = (char *)(*env)->GetStringUTFChars(env, encryptedString, NULL);
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
