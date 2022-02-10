#ifndef QJNIHELPERS_H
#define QJNIHELPERS_H
#include <jni.h>

#include <functional>

namespace QtAndroidPrivate {
typedef std::function<void()> Runnable;
JavaVM *javaVM();
jint initJNI(JavaVM *vm, JNIEnv *env);
jobject classLoader();
jint androidSdkVersion();
}  // namespace QtAndroidPrivate

#endif  // QJNIHELPERS_H
