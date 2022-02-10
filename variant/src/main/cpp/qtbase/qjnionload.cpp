#include <android/log.h>
#include <jni.h>
#include <string.h>

#include "qjnihelpers_p.h"

static const char logTag[] = "QtCore";

jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
  static bool initialized = false;
  if (initialized) return JNI_VERSION_1_6;
  initialized = true;

  typedef union {
    JNIEnv *nenv;
    void *venv;
  } _JNIEnv;

  // __android_log_print(ANDROID_LOG_INFO, logTag, "Start");

  _JNIEnv uenv;
  uenv.venv = nullptr;

  if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_6) != JNI_OK) {
    // __android_log_print(ANDROID_LOG_FATAL, logTag, "GetEnv failed");
    return JNI_ERR;
  }

  JNIEnv *env = uenv.nenv;
  const jint ret = (QtAndroidPrivate::initJNI(vm, env));
  if (ret != 0) {
    // __android_log_print(ANDROID_LOG_FATAL, logTag, "initJNI failed");
    return ret;
  }

  return JNI_VERSION_1_6;
}