#include <android_util.h>
#include <jni.h>
#include <variant.h>

#include <string>

#include "log_utils.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_mgg_ndk_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
  std::string hello = "Hello from C++";
  return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_mgg_ndk_MainActivity_testVariantTest(JNIEnv *env, jobject thiz) {
  std::map<FOREVER::Variant, FOREVER::Variant> g_test_map;
  std::map<FOREVER::Variant, FOREVER::Variant> g_test_complex_map;
  const uint8_t kTestBlobData[] = {89, 0, 65, 198, 4, 99, 0, 9};
  const size_t kTestBlobSize = sizeof(kTestBlobData);  // size in bytes
  const std::vector<FOREVER::Variant> kTestVector = {int64_t(1L), "one", true,
                                                     1.0};
  const std::vector<FOREVER::Variant> kTestComplexVector = {
      int64_t(2L), "two", kTestVector, false, 2.0};
  const std::string kTestMutableString =  // NOLINT
      "I am just great, thanks for asking!";
  g_test_map.clear();
  g_test_map["first"] = 101;
  g_test_map["second"] = 202.2;
  g_test_map["third"] = "three";

  g_test_complex_map.clear();
  g_test_complex_map["one"] = kTestMutableString;
  g_test_complex_map[2] = 123;
  g_test_complex_map[3.0] =
      FOREVER::Variant::FromStaticBlob(kTestBlobData, kTestBlobSize);
  g_test_complex_map[kTestVector] = kTestComplexVector;
  g_test_complex_map[std::string("five")] = g_test_map;
  g_test_complex_map[FOREVER::Variant::FromMutableBlob(
      kTestBlobData, kTestBlobSize)] = kTestMutableString;
  return VariantMapToJavaMap(env, g_test_complex_map);
}

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_ndk_MainActivity_testScalarTypes(JNIEnv *env, jobject thiz) {
  {
    FOREVER::Variant v;
    LOGE("%d", v.is_null());
    LOGE("%d", v.is_fundamental_type());
    LOGE("%d ", v.is_container_type());
  }

  {
    const int64_t kTestInt64 = 12345L;
    FOREVER::Variant v(kTestInt64);
    LOGE("%d", v.int64_value());
    LOGE("%d", v.is_int64());
    LOGE("%d", v.is_null());
    LOGE("%d", v.is_fundamental_type());
    LOGE("%d ", v.is_container_type());
  }
}