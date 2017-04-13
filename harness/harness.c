/*
 * Native Harness
 *
 * Starts a socket server which exposes a native library function.
 *
 * Original idea from native-shim by Tim "diff" Strazere
 * https://github.com/rednaga/native-shim
 *
*/

#include <stdlib.h>
#include <stdio.h>

#include <dlfcn.h>
#include <unistd.h>

#include "vm.h"
#include "server.h"

#define PORT_NO 5001


typedef int(*JNI_OnLoadFunc)(JavaVM *vm, void *reserved);

int main(int argc, const char * argv[]) {
  printf("[*] Native Harness\n");

  printf(" [+] Loading target: [ %s ]\n", argv[1]);
  if (access(argv[1], F_OK) == -1) {
    printf(" [!] File does not exist!\n");
    return -1;
  }

  void * handle = dlopen(argv[1], RTLD_LAZY);
  if (handle == NULL) {
    printf(" [!] Could not dlopen file! (%s)\n", dlerror());
    return -1;
  }
  printf(" [+] Library Loaded!\n");

  JNI_OnLoadFunc onLoadFunc = dlsym(handle, "JNI_OnLoad");
  if (onLoadFunc == NULL) {
    printf(" [!] No JNI_OnLoad found!\n");
    return -1;
  }
  printf(" [+] Found JNI_OnLoad, good\n");

  printf(" [+] Initializing JavaVM Instance\n");
  JavaVM *vm = NULL;
  JNIEnv *env = NULL;
  int status = init_jvm(&vm, &env);
  if (status == 0) {
    printf(" [+] Initialization success (vm=%p, env=%p)\n", vm, env);
  } else {
    printf(" [!] Initialization failure (%i)\n", status);
    return -1;
  }

  printf(" [+] Calling JNI_OnLoad\n");
  onLoadFunc(vm, NULL);

  decryptString_t decryptString = dlsym(handle, "Java_org_cf_nativeharness_Cryptor_decryptString");
  if (decryptString == NULL) {
    printf(" [!] Couldn't find decryptString function\n");
  } else {
    printf(" [+] Found decryptString function, good (%p)\n", decryptString);
  }

  // Because decryptString is an instance method, to properly call the method, need instance of class.
  // If decryptString were static, could just pass NULL for instance.
  printf(" [+] Finding Cryptor class\n");
  jclass cryptor_class;
  cryptor_class = (*env)->FindClass(env, "org.cf.nativeharness.Cryptor");
  if (cryptor_class == NULL) {
    printf(" [!] Couldn't find Cryptor on the class path\n");
    return -1;
  } else {
    printf(" [+] Found Cryptor class: %p\n", cryptor_class);
  }

  jmethodID getinstance_method;
  getinstance_method = (*env)->GetStaticMethodID(env, cryptor_class, "getInstance", "()Lorg/cf/nativeharness/Cryptor;");
  if (getinstance_method == NULL) {
    printf(" [!] Could not find Cryptor.getInstance() method\n");
    return -1;
  } else {
    printf(" [+] Found Cryptor.getInstance(): %p\n", getinstance_method);
  }

  jobject cryptor_instance;
  cryptor_instance = (*env)->CallStaticObjectMethod(env, cryptor_class, getinstance_method, NULL);
  if (cryptor_instance == NULL) {
    printf(" [!] Couldn't call Cryptor.getInstance()\n");
    return -1;
  } else {
    printf(" [+] Instantiated Cryptor class: %p\n", cryptor_instance);
  }

  printf(" [+] Starting socket server on port %i\n", PORT_NO);
  start_server(PORT_NO, vm, env, decryptString, cryptor_instance);

  printf(" [+] Cleaning up VM\n");
  (*vm)->DestroyJavaVM(vm);

  printf(" [+] Closing target library\n");
  dlclose(handle);

  return 0;
}
