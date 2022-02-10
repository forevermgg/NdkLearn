#include "qjnihelpers_p.h"

static JavaVM *g_javaVM = nullptr;
static jobject g_jClassLoader = nullptr;
static jint g_androidSdkVersion = 0;
static jclass g_jNativeClass = nullptr;
static jmethodID g_runPendingCppRunnablesMethodID = nullptr;

static inline bool exceptionCheck(JNIEnv *env) {
  if (env->ExceptionCheck()) {
    env->ExceptionDescribe();
    env->ExceptionClear();
    return true;
  }
  return false;
}

static void setAndroidSdkVersion(JNIEnv *env) {
  jclass androidVersionClass = env->FindClass("android/os/Build$VERSION");
  if (exceptionCheck(env)) return;

  jfieldID androidSDKFieldID =
      env->GetStaticFieldID(androidVersionClass, "SDK_INT", "I");
  if (exceptionCheck(env)) return;

  g_androidSdkVersion =
      env->GetStaticIntField(androidVersionClass, androidSDKFieldID);
}

// function called from Java from Android UI thread
static void runPendingCppRunnables(JNIEnv * /*env*/, jobject /*obj*/) {}

jint QtAndroidPrivate::initJNI(JavaVM *vm, JNIEnv *env) {
  jclass jQtNative = env->FindClass("com/mgg/ndk/QtNative");

  if (exceptionCheck(env)) return JNI_ERR;

  jmethodID classLoaderMethodID = env->GetStaticMethodID(
      jQtNative, "classLoader", "()Ljava/lang/ClassLoader;");

  if (exceptionCheck(env)) return JNI_ERR;

  jobject classLoader =
      env->CallStaticObjectMethod(jQtNative, classLoaderMethodID);
  if (exceptionCheck(env)) return JNI_ERR;

  setAndroidSdkVersion(env);

  g_jClassLoader = env->NewGlobalRef(classLoader);
  env->DeleteLocalRef(classLoader);
  g_javaVM = vm;

  static const JNINativeMethod methods[] = {
      {"runPendingCppRunnables", "()V",
       reinterpret_cast<void *>(runPendingCppRunnables)},
  };

  const bool regOk =
      (env->RegisterNatives(jQtNative, methods,
                            sizeof(methods) / sizeof(methods[0])) == JNI_OK);

  if (!regOk && exceptionCheck(env)) return JNI_ERR;

  g_runPendingCppRunnablesMethodID = env->GetStaticMethodID(
      jQtNative, "runPendingCppRunnablesOnAndroidThread", "()V");
  g_jNativeClass = static_cast<jclass>(env->NewGlobalRef(jQtNative));
  env->DeleteLocalRef(jQtNative);
  return JNI_OK;
}

JavaVM *QtAndroidPrivate::javaVM() { return g_javaVM; }

jobject QtAndroidPrivate::classLoader() { return g_jClassLoader; }

jint QtAndroidPrivate::androidSdkVersion() { return g_androidSdkVersion; }
