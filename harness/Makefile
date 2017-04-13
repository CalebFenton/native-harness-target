LOCAL_ARM_MODE := arm armeabi armeabi-v7a x86

all:
	ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk

install:
	adb push libs/x86/harness /data/local/tmp/

clean:
	rm -fr *~ obj/ libs/