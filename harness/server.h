#ifndef SERVER_H
#define SERVER_H

#define MAX_LINE_LEN 2048

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <jni.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
#include <errno.h>

// Target function type signature
typedef jstring(*decryptString_t)(JNIEnv *, jobject, jstring);

void start_server(int port, JavaVM *vm, JNIEnv *env, decryptString_t decryptString, jobject cryptor_instance);
static ssize_t read_line(int fd, void *buffer, size_t n);

#endif