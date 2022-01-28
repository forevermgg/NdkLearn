#include <android_util.h>
#include <jni.h>
#include <variant.h>
#include "spdlog/spdlog.h"
#include <string>
#include "spdlog/sinks/android_sink.h"
#include "json.hpp"
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

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_ndk_MainActivity_testJson(JNIEnv *env, jobject thiz) {
  {
    // create an empty structure (null)
    nlohmann::json j;

    // add a number that is stored as double (note the implicit conversion of j
    // to an object)
    j["pi"] = 3.141;

    // add a Boolean that is stored as bool
    j["happy"] = true;

    // add a string that is stored as std::string
    j["name"] = "Niels";

    // add another null object by passing nullptr
    j["nothing"] = nullptr;

    // add an object inside the object
    j["answer"]["everything"] = 42;

    // add an array that is stored as std::vector (using an initializer list)
    j["list"] = {1, 0, 2};

    // add another object (using an initializer list of pairs)
    j["object"] = {{"currency", "USD"}, {"value", 42.99}};

    // instead, you could also write (which looks very similar to the JSON
    // above)
    nlohmann::json j2 = {{"pi", 3.141},
                         {"happy", true},
                         {"name", "Niels"},
                         {"nothing", nullptr},
                         {"answer", {{"everything", 42}}},
                         {"list", {1, 0, 2}},
                         {"object", {{"currency", "USD"}, {"value", 42.99}}}};

    std::string str = j2.dump();
    LOGE("%s", str.c_str());
  }

  {
    // create object from string literal
    nlohmann::json j = "{ \"happy\": true, \"pi\": 3.141 }"_json;
    LOGE("%s", j.dump().c_str());
    // or even nicer with a raw string literal
    auto j2 = R"(
  {
    "happy": true,
    "pi": 3.141
  })"_json;
    LOGE("%s", j2.dump().c_str());
    // parse explicitly
    auto j3 = nlohmann::json::parse("{ \"happy\": true, \"pi\": 3.141 }");
    LOGE("%s", j3.dump().c_str());
    // explicit conversion to string
    std::string s = j.dump();  // {\"happy\":true,\"pi\":3.141}

    // serialization with pretty printing
    // pass in the amount of spaces to indent
    LOGE("%s", j.dump(5).c_str());
    // {
    //     "happy": true,
    //     "pi": 3.141
    // }
  }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_ndk_MainActivity_testLog(JNIEnv *env, jobject thiz) {
  std::string tag = "spdlog-android";
  auto android_logger = spdlog::android_logger_mt("android", tag);
  android_logger->set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
  android_logger->critical("Use \"adb shell logcat\" to view this message.");
}
