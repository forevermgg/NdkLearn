#include <jni.h>

#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <variant>

#include "variant/android_util.h"
#include "variant/variant.h"

static void finalize_mixed(jlong ptr) {
  delete reinterpret_cast<FOREVER::Variant *>(ptr);
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeGetFinalizerPtr(JNIEnv *,
                                                                 jclass) {
  return reinterpret_cast<jlong>(&finalize_mixed);
}

extern "C" JNIEXPORT jint JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeGetType(JNIEnv *env,
                                                         jclass clazz,
                                                         jlong native_ptr) {
  auto variant_value = reinterpret_cast<FOREVER::Variant *>(native_ptr);
  return variant_value->type();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeEquals(
    JNIEnv *env, jclass clazz, jlong native_ptr, jlong native_other_ptr) {
  try {
    auto java_value = *reinterpret_cast<FOREVER::Variant *>(native_ptr);
    auto other_java_value =
        *reinterpret_cast<FOREVER::Variant *>(native_other_ptr);
    return java_value.type() == other_java_value.type();
  } catch (const std::exception) {
  }
  return false;
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeCreateNull(JNIEnv *env,
                                                            jclass clazz) {
  return reinterpret_cast<jlong>(new FOREVER::Variant());
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeCreateBoolean(JNIEnv *env,
                                                               jclass clazz,
                                                               jboolean value) {
  auto bool_value = (bool)(value == JNI_TRUE);
  return reinterpret_cast<jlong>(new FOREVER::Variant(bool_value));
}

extern "C" JNIEXPORT jboolean JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeAsBoolean(JNIEnv *env,
                                                           jclass clazz,
                                                           jlong native_ptr) {
  auto variant_value = reinterpret_cast<FOREVER::Variant *>(native_ptr);
  if (!variant_value) {
    return JNI_FALSE;
  } else {
    return variant_value->bool_value() ? JNI_TRUE : JNI_FALSE;
  }
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeCreateLong(JNIEnv *env,
                                                            jclass clazz,
                                                            jlong value) {
  auto long_value = (int64_t) reinterpret_cast<int64_t>(value);
  return reinterpret_cast<jlong>(new FOREVER::Variant(long_value));
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeAsLong(JNIEnv *env,
                                                        jclass clazz,
                                                        jlong native_ptr) {
  auto variant_value = reinterpret_cast<FOREVER::Variant *>(native_ptr);
  if (!variant_value) {
    return jlong(0);
  } else {
    return variant_value->int64_value();
  }
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeCreateFloat(JNIEnv *env,
                                                             jclass clazz,
                                                             jfloat value) {
  auto float_value = (float)(value);
  return reinterpret_cast<jlong>(new FOREVER::Variant(float_value));
}

extern "C" JNIEXPORT jfloat JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeAsFloat(JNIEnv *env,
                                                         jclass clazz,
                                                         jlong native_ptr) {
  auto variant_value = reinterpret_cast<FOREVER::Variant *>(native_ptr);
  if (!variant_value) {
    return jfloat(0);
  } else {
    return variant_value->double_value();
  }
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeCreateDouble(JNIEnv *env,
                                                              jclass clazz,
                                                              jdouble value) {
  auto double_value = (double)(value);
  return reinterpret_cast<jlong>(new FOREVER::Variant(double_value));
}

extern "C" JNIEXPORT jdouble JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeAsDouble(JNIEnv *env,
                                                          jclass clazz,
                                                          jlong native_ptr) {
  auto variant_value = reinterpret_cast<FOREVER::Variant *>(native_ptr);
  if (!variant_value) {
    return jfloat(0);
  } else {
    return variant_value->double_value();
  }
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeCreateString(JNIEnv *env,
                                                              jclass clazz,
                                                              jstring value) {
  auto string_value = JniStringToString(env, value);
  return reinterpret_cast<jlong>(new FOREVER::Variant(string_value));
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeAsString(JNIEnv *env,
                                                          jclass clazz,
                                                          jlong native_ptr) {
  auto variant_value = reinterpret_cast<FOREVER::Variant *>(native_ptr);
  if (!variant_value) {
    return env->NewStringUTF("");
  } else {
    const char *char_value = variant_value->string_value();
    return env->NewStringUTF(char_value);
  }
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeCreateDate(JNIEnv *env,
                                                            jclass clazz,
                                                            jlong value) {
  return reinterpret_cast<jlong>(new FOREVER::Variant(value));
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeAsDate(JNIEnv *env,
                                                        jclass clazz,
                                                        jlong native_ptr) {
  auto java_value = *reinterpret_cast<FOREVER::Variant *>(native_ptr);
  return java_value.int64_value();
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeCreateUUID(JNIEnv *env,
                                                            jclass clazz,
                                                            jstring value) {
  auto string_value = JniStringToString(env, value);
  return reinterpret_cast<jlong>(new FOREVER::Variant(string_value));
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeAsUUID(JNIEnv *env,
                                                        jclass clazz,
                                                        jlong native_ptr) {
  auto variant_value = reinterpret_cast<FOREVER::Variant *>(native_ptr);
  if (!variant_value) {
    return env->NewStringUTF("");
  } else {
    const char *char_value = variant_value->string_value();
    return env->NewStringUTF(char_value);
  }
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeCreateObjectId(JNIEnv *env,
                                                                jclass clazz,
                                                                jstring value) {
  auto string_value = JniStringToString(env, value);
  return reinterpret_cast<jlong>(new FOREVER::Variant(string_value));
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_realm_internal_core_NativeRealmAny_nativeAsObjectId(JNIEnv *env,
                                                            jclass clazz,
                                                            jlong native_ptr) {
  auto variant_value = reinterpret_cast<FOREVER::Variant *>(native_ptr);
  if (!variant_value) {
    return env->NewStringUTF("");
  } else {
    const char *char_value = variant_value->string_value();
    return env->NewStringUTF(char_value);
  }
}