#include <jni.h>

#include <iostream>

typedef void (*FinalizeFunc)(jlong);

extern "C" JNIEXPORT void JNICALL
Java_io_realm_internal_NativeObjectReference_nativeCleanUp(JNIEnv *, jclass,
                                                           jlong finalizer_ptr,
                                                           jlong native_ptr) {
  FinalizeFunc finalize_func = reinterpret_cast<FinalizeFunc>(finalizer_ptr);
  finalize_func(native_ptr);
  std::cout << "finalize_func" << std::endl;
}