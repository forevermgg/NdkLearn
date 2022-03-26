#include <jni.h>
#include <string>
#include "log_settings.h"
#include "logging.h"
#include "scoped_java_ref.h"
#include "jni_util.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_log_Logger_initConsoleLog(JNIEnv *env, jclass clazz) {
    Forever::LogSettings log_settings;
    log_settings.min_log_level = Forever::LOG_INFO;
    Forever::SetLogSettings(log_settings);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_log_Logger_testPrintLog(JNIEnv *env, jclass clazz) {
    const char* msg1 = "test message";
    const char* msg2 = "hello";
    const char* msg3 = "logging";
    const char* msg4 = "Another message";
    const char* msg5 = "Foo";

    Forever::jni::ScopedJavaLocalRef<jclass> string_class(
            env, env->FindClass("java/lang/String"));
    if (string_class.is_null()) {
        FML_LOG(ERROR) << "Could not locate String class";
    }

    FML_LOG(INFO) << msg1;
    FML_LOG(WARNING) << msg2;
    FML_LOG(ERROR) << msg3;
    FML_VLOG(1) << msg4;
    // VLOG(2) is not enabled so the log gets dropped.
    FML_VLOG(2) << msg5;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_log_Logger_initLogger(JNIEnv *env, jclass clazz, jstring path_dir) {
    Forever::jni::JavaStringToString(env, path_dir);
}