#include <jni.h>
#include <string>
#include "log_settings.h"
#include "logging.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_log_Logging_initConsoleLog(JNIEnv *env, jclass clazz) {
    Forever::LogSettings log_settings;
    log_settings.min_log_level = Forever::LOG_INFO;
    Forever::SetLogSettings(log_settings);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_log_Logging_testPrintLog(JNIEnv *env, jclass clazz) {
    const char* msg1 = "test message";
    const char* msg2 = "hello";
    const char* msg3 = "logging";
    const char* msg4 = "Another message";
    const char* msg5 = "Foo";

    FML_LOG(INFO) << msg1;
    FML_LOG(WARNING) << msg2;
    FML_LOG(ERROR) << msg3;
    FML_VLOG(1) << msg4;
    // VLOG(2) is not enabled so the log gets dropped.
    FML_VLOG(2) << msg5;
}