#include <jni.h>
#include <string>
#include <client/linux/handler/exception_handler.h>
#include <client/linux/handler/minidump_descriptor.h>

google_breakpad::ExceptionHandler *exceptionHandler = nullptr;

bool DumpCallback(const google_breakpad::MinidumpDescriptor &descriptor,
                  void *context,
                  bool succeeded) {
    return succeeded;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_breakpad_Breakpad_initBreakpad(JNIEnv *env, jclass clazz, jstring _path) {
    const char *path = env->GetStringUTFChars(_path, 0);
    if (exceptionHandler == nullptr) {
        google_breakpad::MinidumpDescriptor descriptor(path);
        exceptionHandler = new google_breakpad::ExceptionHandler(descriptor, NULL, DumpCallback,
                                                                 NULL, true, -1);
    }
    env->ReleaseStringUTFChars(_path, path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_breakpad_Breakpad_unitBreakpad(JNIEnv *env, jclass clazz, jstring path) {
    if (exceptionHandler != nullptr) {
        delete exceptionHandler;
        exceptionHandler = nullptr;
    }
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    return JNI_VERSION_1_4;
}

[[maybe_unused]] JNIEXPORT void JNI_OnUnLoad(JavaVM *vm, void *reserved) {

}
