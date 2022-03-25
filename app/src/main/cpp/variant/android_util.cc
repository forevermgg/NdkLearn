#include "android_util.h"

// Check for JNI exceptions, report them if any, and clear them.
bool CheckAndClearJniExceptions(JNIEnv* env) {
  if (env->ExceptionCheck()) {
    env->ExceptionDescribe();
    env->ExceptionClear();
    return true;
  }
  return false;
}

bool IsJArray(JNIEnv* env, jobject obj) {
  jclass class_jclass = env->FindClass("java/lang/Class");
  /*jmethodID class_get_name_method_id =
      env->GetMethodID(class_jclass, "getName", "()Ljava/lang/String;");*/
  jmethodID class_is_array_method_id =
      env->GetMethodID(class_jclass, "isArray", "()Z");
  jclass obj_class = env->GetObjectClass(obj);
  const bool is_array =
      env->CallBooleanMethod(obj_class, class_is_array_method_id);
  CheckAndClearJniExceptions(env);
  env->DeleteLocalRef(obj_class);
  return is_array;
}

std::string JObjectClassName(JNIEnv* env, jobject obj) {
  jclass class_jclass = env->FindClass("java/lang/Class");
  jmethodID class_get_name_method_id =
      env->GetMethodID(class_jclass, "getName", "()Ljava/lang/String;");
  /*jmethodID class_is_array_method_id =
      env->GetMethodID(class_jclass, "isArray", "()Z");*/
  jclass obj_class = env->GetObjectClass(obj);
  jobject obj_name = env->CallObjectMethod(obj_class, class_get_name_method_id);
  CheckAndClearJniExceptions(env);
  return JniStringToString(env, obj_name);
}

// Converts a `std::vector<std::string>` to a `java.util.ArrayList<String>`
// Returns a local ref to a List.
jobject StdVectorToJavaList(JNIEnv* env,
                            const std::vector<std::string>& string_vector) {
  jclass array_list_class = env->FindClass("java/util/ArrayList");
  jmethodID array_list_constructor_method_id =
      env->GetMethodID(array_list_class, "<init>", "()V");
  /*jmethodID array_list_constructor_with_size_method_id =
      env->GetMethodID(array_list_class, "<init>", "(I)V");*/
  jmethodID array_list_add_method_id =
      env->GetMethodID(array_list_class, "add", "(Ljava/lang/Object;)Z");
  jobject java_list =
      env->NewObject(array_list_class, array_list_constructor_method_id);
  jmethodID add_method = array_list_add_method_id;
  for (const auto & it : string_vector) {
    jstring value = env->NewStringUTF(it.c_str());
    env->CallBooleanMethod(java_list, add_method, value);
    CheckAndClearJniExceptions(env);
    env->DeleteLocalRef(value);
  }
  return java_list;
}

// Converts an `std::map<const char*, const char*>` to a
// `java.util.Map<String, String>`.
void StdMapToJavaMap(JNIEnv* env, jobject* to,
                     const std::map<const char*, const char*>& string_map) {
  jclass map_class = env->FindClass("java/util/Map");
  // jmethodID map_size_method_id = env->GetMethodID(map_class, "size", "()I");
  /*jmethodID map_get_method_id = env->GetMethodID(
      map_class, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");*/
  jmethodID map_put_method_id = env->GetMethodID(
      map_class, "put",
      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
  /*jmethodID map_remove_method_id = env->GetMethodID(
      map_class, "remove", "(Ljava/lang/Object;)Ljava/lang/Object;");*/
  for (auto it : string_map) {
    jstring key = env->NewStringUTF(it.first);
    jstring value = env->NewStringUTF(it.second);
    jobject previous =
        env->CallObjectMethod(*to, map_put_method_id, key, value);
    CheckAndClearJniExceptions(env);
    if (previous) env->DeleteLocalRef(previous);
    env->DeleteLocalRef(value);
    env->DeleteLocalRef(key);
  }
}

// Converts an `std::map<std::string, std::string>` to a
// `java.util.Map<String, String>`.
void StdMapToJavaMap(JNIEnv* env, jobject* to,
                     const std::map<std::string, std::string>& from) {
  jclass map_class = env->FindClass("java/util/Map");
  jmethodID map_put_method_id = env->GetMethodID(
      map_class, "put",
      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
  for (const auto & item : from) {
    jstring key = env->NewStringUTF(item.first.c_str());
    jstring value = env->NewStringUTF(item.second.c_str());
    jobject previous =
        env->CallObjectMethod(*to, map_put_method_id, key, value);
    CheckAndClearJniExceptions(env);
    if (previous) env->DeleteLocalRef(previous);
    env->DeleteLocalRef(value);
    env->DeleteLocalRef(key);
  }
}

// Converts a `java.util.Map<String, String>` to an
// `std::map<std::string, std::string>`.
// T is the type of the map. Our current requirements always have the same Key
// and Value types, so just specify one type.
// ConvertFn is of type:  T Function(JNIEnv* env, jobject obj)
template <typename T, typename ConvertFn>
static void JavaMapToStdMapTemplate(JNIEnv* env, std::map<T, T>* to,
                                    jobject from, ConvertFn convert) {
  jclass set_class = env->FindClass("java/util/Set");
  jmethodID set_iterator_method_id =
      env->GetMethodID(set_class, "iterator", "()Ljava/util/Iterator;");
  jclass map_class = env->FindClass("java/util/Map");
  jmethodID map_key_set_method_id =
      env->GetMethodID(map_class, "keySet", "()Ljava/util/Set;");
  jmethodID map_get_method_id = env->GetMethodID(
      map_class, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
  jclass iterator_class = env->FindClass("java/util/Iterator");
  jmethodID iterator_has_next_method_id =
      env->GetMethodID(iterator_class, "hasNext", "()Z");
  jmethodID iterator_next_method_id =
      env->GetMethodID(iterator_class, "next", "()Ljava/lang/Object;");
  jobject key_set = env->CallObjectMethod(from, map_key_set_method_id);
  CheckAndClearJniExceptions(env);
  // Iterator iter = key_set.iterator();
  jobject iter = env->CallObjectMethod(key_set, set_iterator_method_id);
  CheckAndClearJniExceptions(env);
  // while (iter.hasNext())
  while (env->CallBooleanMethod(iter, iterator_has_next_method_id)) {
    CheckAndClearJniExceptions(env);
    // T key = iter.next();
    // T value = from.get(key);
    jobject key_object = env->CallObjectMethod(iter, iterator_next_method_id);
    CheckAndClearJniExceptions(env);
    jobject value_object =
        env->CallObjectMethod(from, map_get_method_id, key_object);
    CheckAndClearJniExceptions(env);
    const T key = convert(env, key_object);
    const T value = convert(env, value_object);
    env->DeleteLocalRef(key_object);
    env->DeleteLocalRef(value_object);

    to->insert(std::pair<T, T>(key, value));
  }
  env->DeleteLocalRef(iter);
  env->DeleteLocalRef(key_set);
}

// Converts a `java.util.Map<String, String>` to an
// `std::map<std::string, std::string>`.
void JavaMapToStdMap(JNIEnv* env, std::map<std::string, std::string>* to,
                     jobject from) {
  JavaMapToStdMapTemplate<std::string>(env, to, from, JStringToString);
}

// Converts a `java.util.Map<java.lang.Object, java.lang.Object>` to an
// `std::map<Variant, Variant>`.
void JavaMapToVariantMap(JNIEnv* env,
                         std::map<FOREVER::Variant, FOREVER::Variant>* to,
                         jobject from) {
  JavaMapToStdMapTemplate<FOREVER::Variant>(env, to, from, JavaObjectToVariant);
}

// Converts a `java.util.Set<String>` to a `std::vector<std::string>`.
void JavaSetToStdStringVector(JNIEnv* env, std::vector<std::string>* to,
                              jobject from) {
  jclass jclass_iterator = env->FindClass("java/util/Iterator");
  jmethodID iterator_has_next_method_id =
      env->GetMethodID(jclass_iterator, "hasNext", "()Z");
  jmethodID iterator_next_method_id =
      env->GetMethodID(jclass_iterator, "next", "()Ljava/lang/Object;");
  jmethodID iterator_method_id =
      env->GetMethodID(jclass_iterator, "iterator", "()Ljava/util/Iterator;");
  jobject iter = env->CallObjectMethod(from, iterator_method_id);
  CheckAndClearJniExceptions(env);
  // while (iter.hasNext())
  while (env->CallBooleanMethod(iter, iterator_has_next_method_id)) {
    CheckAndClearJniExceptions(env);
    // String elem = iter.next();
    jobject elem_object = env->CallObjectMethod(iter, iterator_next_method_id);
    CheckAndClearJniExceptions(env);
    std::string elem = JniStringToString(env, elem_object);
    to->push_back(elem);
  }
  env->DeleteLocalRef(iter);
}

// Converts a `java.util.List<String>` to a `std::vector<std::string>`.
void JavaListToStdStringVector(JNIEnv* env, std::vector<std::string>* vector,
                               jobject from) {
  jclass array_list_class = env->FindClass("java/util/List");
  jmethodID list_size_method_id =
      env->GetMethodID(array_list_class, "size", "()I");
  jmethodID list_get_method_id =
      env->GetMethodID(array_list_class, "get", "(I)Ljava/lang/Object;");
  /*jmethodID list_set_method_id = env->GetMethodID(
      array_list_class, "set", "(ILjava/lang/Object;)Ljava/lang/Object;");*/
  int size = env->CallIntMethod(from, list_size_method_id);
  CheckAndClearJniExceptions(env);
  vector->clear();
  vector->reserve(size);
  for (int i = 0; i < size; i++) {
    jobject element = env->CallObjectMethod(from, list_get_method_id, i);
    CheckAndClearJniExceptions(env);
    vector->push_back(JniStringToString(env, element));
  }
}

// Converts a `java.util.List<Object>` to a `std::vector<std::string>`.
void JavaObjectListToStdStringVector(JNIEnv* env, std::vector<std::string>* to,
                                     jobject from) {
  jclass array_list_class = env->FindClass("java/util/List");
  jmethodID list_size_method_id =
      env->GetMethodID(array_list_class, "size", "()I");
  jmethodID list_get_method_id =
      env->GetMethodID(array_list_class, "get", "(I)Ljava/lang/Object;");
  /*jmethodID list_set_method_id = env->GetMethodID(
      array_list_class, "set", "(ILjava/lang/Object;)Ljava/lang/Object;");*/
  int size = env->CallIntMethod(from, list_size_method_id);
  CheckAndClearJniExceptions(env);
  to->clear();
  to->reserve(size);
  for (int i = 0; i < size; i++) {
    jobject element = env->CallObjectMethod(from, list_get_method_id, i);
    CheckAndClearJniExceptions(env);
    to->push_back(JniObjectToString(env, element));
    env->DeleteLocalRef(element);
  }
}

// Convert a jstring to a std::string, releasing the reference to the
// jstring.
std::string JniStringToString(JNIEnv* env, jobject string_object) {
  std::string return_string = JStringToString(env, string_object);
  env->DeleteLocalRef(string_object);
  return return_string;
}

// Convert a Java object of type java.lang.Object into an std::string, by
// calling toString(), then release the object.
std::string JniObjectToString(JNIEnv* env, jobject obj) {
  if (obj == nullptr) return "";
  jclass object_class = env->FindClass("java/lang/Object");
  jmethodID object_to_string_method_id =
      env->GetMethodID(object_class, "toString", "()Ljava/lang/String;");
  jobject str = env->CallObjectMethod(obj, object_to_string_method_id);
  CheckAndClearJniExceptions(env);
  return JniStringToString(env, str);
}

// Convert a jstring (created by the JVM e.g passed into a native method)
// into
// a std::string.  Unlike JniStringToString() this does not release the
// reference to the string_object as the caller owns the object in a native
// method.
std::string JStringToString(JNIEnv* env, jobject string_object) {
  if (string_object == nullptr) return "";
  const char* string_buffer =
      env->GetStringUTFChars(reinterpret_cast<jstring>(string_object), 0);
  std::string return_string(string_buffer);
  env->ReleaseStringUTFChars(reinterpret_cast<jstring>(string_object),
                             string_buffer);
  return return_string;
}

bool JBooleanToBool(JNIEnv* env, jobject obj) {
  jclass boolean_class = env->FindClass("java/lang/Boolean");
  /*jmethodID bool_constructor_method_id =
      env->GetMethodID(boolean_class, "<init>", "(Z)V");*/
  jmethodID bool_value_method_id =
      env->GetMethodID(boolean_class, "booleanValue", "()Z");
  CheckAndClearJniExceptions(env);
  return env->CallBooleanMethod(obj, bool_value_method_id);
}

uint8_t JByteToUInt8(JNIEnv* env, jobject obj) {
  jclass byte_class = env->FindClass("java/lang/Byte");
  /*jmethodID byte_constructor_method_id =
      env->GetMethodID(byte_class, "<init>", "(B)V");*/
  jmethodID byte_value_method_id =
      env->GetMethodID(byte_class, "byteValue", "()B");
  CheckAndClearJniExceptions(env);
  return env->CallByteMethod(obj, byte_value_method_id);
}

char JCharToChar(JNIEnv* env, jobject obj) {
  jclass char_class = env->FindClass("java/lang/Character");
  /*jmethodID char_constructor_method_id =
      env->GetMethodID(char_class, "<init>", "(C)V");*/
  jmethodID char_value_method_id =
      env->GetMethodID(char_class, "charValue", "()C");
  CheckAndClearJniExceptions(env);
  char ret = env->CallCharMethod(obj, char_value_method_id);
  return ret;
}

int16_t JShortToInt16(JNIEnv* env, jobject obj) {
  jclass short_class = env->FindClass("java/lang/Short");
  /*jmethodID short_constructor_method_id =
      env->GetMethodID(short_class, "<init>", "(S)V");*/
  jmethodID short_value_method_id =
      env->GetMethodID(short_class, "shortValue", "()S");
  CheckAndClearJniExceptions(env);
  return env->CallShortMethod(obj, short_value_method_id);
}

int JIntegerToInt(JNIEnv* env, jobject obj) {
  jclass int_class = env->FindClass("java/lang/Integer");
  /*jmethodID int_constructor_method_id =
      env->GetMethodID(int_class, "<init>", "(I)V");*/
  jmethodID int_value_method_id =
      env->GetMethodID(int_class, "intValue", "()I");
  CheckAndClearJniExceptions(env);
  return env->CallIntMethod(obj, int_value_method_id);
}

int64_t JLongToInt64(JNIEnv* env, jobject obj) {
  jclass long_class = env->FindClass("java/lang/Long");
  /*jmethodID long_constructor_method_id =
      env->GetMethodID(long_class, "<init>", "(J)V");*/
  jmethodID long_value_method_id =
      env->GetMethodID(long_class, "longValue", "()J");
  CheckAndClearJniExceptions(env);
  return env->CallLongMethod(obj, long_value_method_id);
}

float JFloatToFloat(JNIEnv* env, jobject obj) {
  jclass float_class = env->FindClass("java/lang/Float");
  /*jmethodID float_constructor_method_id =
      env->GetMethodID(float_class, "<init>", "(F)V");*/
  jmethodID float_value_method_id =
      env->GetMethodID(float_class, "floatValue", "()F");
  CheckAndClearJniExceptions(env);
  return env->CallFloatMethod(obj, float_value_method_id);
}

double JDoubleToDouble(JNIEnv* env, jobject obj) {
  jclass double_class = env->FindClass("java/lang/Double");
  /*jmethodID double_constructor_method_id =
      env->GetMethodID(double_class, "<init>", "(D)V");*/
  jmethodID double_value_method_id =
      env->GetMethodID(double_class, "doubleValue", "()D");
  CheckAndClearJniExceptions(env);
  return env->CallDoubleMethod(obj, double_value_method_id);
}

FOREVER::Variant JBooleanArrayToVariant(JNIEnv* env, jbooleanArray array) {
  const size_t len = env->GetArrayLength(array);
  jboolean* c_array = env->GetBooleanArrayElements(array, nullptr);
  auto vec = new std::vector<FOREVER::Variant>(len);
  for (size_t i = 0; i < len; ++i) {
    (*vec)[i] = FOREVER::Variant(static_cast<bool>(c_array[i]));
  }
  FOREVER::Variant v;
  v.AssignVector(&vec);
  env->ReleaseBooleanArrayElements(array, c_array, JNI_ABORT);
  return v;
}

FOREVER::Variant JByteArrayToVariant(JNIEnv* env, jbyteArray array) {
  const size_t len = env->GetArrayLength(array);
  jbyte* c_array = env->GetByteArrayElements(array, nullptr);
  auto vec = new std::vector<FOREVER::Variant>(len);
  for (size_t i = 0; i < len; ++i) {
    (*vec)[i] = FOREVER::Variant(static_cast<uint8_t>(c_array[i]));
  }
  FOREVER::Variant v;
  v.AssignVector(&vec);
  env->ReleaseByteArrayElements(array, c_array, JNI_ABORT);
  return v;
}

FOREVER::Variant JCharArrayToVariant(JNIEnv* env, jcharArray array) {
  const size_t len = env->GetArrayLength(array);
  jchar* c_array = env->GetCharArrayElements(array, nullptr);
  auto vec = new std::vector<FOREVER::Variant>(len);
  for (size_t i = 0; i < len; ++i) {
    (*vec)[i] = FOREVER::Variant(static_cast<char>(c_array[i]));
  }
  FOREVER::Variant v;
  v.AssignVector(&vec);
  env->ReleaseCharArrayElements(array, c_array, JNI_ABORT);
  return v;
}

FOREVER::Variant JShortArrayToVariant(JNIEnv* env, jshortArray array) {
  const size_t len = env->GetArrayLength(array);
  jshort* c_array = env->GetShortArrayElements(array, nullptr);
  auto vec = new std::vector<FOREVER::Variant>(len);
  for (size_t i = 0; i < len; ++i) {
    (*vec)[i] = FOREVER::Variant(static_cast<int16_t>(c_array[i]));
  }
  FOREVER::Variant v;
  v.AssignVector(&vec);
  env->ReleaseShortArrayElements(array, c_array, JNI_ABORT);
  return v;
}

FOREVER::Variant JIntArrayToVariant(JNIEnv* env, jintArray array) {
  const size_t len = env->GetArrayLength(array);
  jint* c_array = env->GetIntArrayElements(array, nullptr);
  auto vec = new std::vector<FOREVER::Variant>(len);
  for (size_t i = 0; i < len; ++i) {
    (*vec)[i] = FOREVER::Variant(static_cast<int>(c_array[i]));
  }
  FOREVER::Variant v;
  v.AssignVector(&vec);
  env->ReleaseIntArrayElements(array, c_array, JNI_ABORT);
  return v;
}

FOREVER::Variant JLongArrayToVariant(JNIEnv* env, jlongArray array) {
  const size_t len = env->GetArrayLength(array);
  jlong* c_array = env->GetLongArrayElements(array, nullptr);
  auto vec = new std::vector<FOREVER::Variant>(len);
  for (size_t i = 0; i < len; ++i) {
    (*vec)[i] = FOREVER::Variant(static_cast<int64_t>(c_array[i]));
  }
  FOREVER::Variant v;
  v.AssignVector(&vec);
  env->ReleaseLongArrayElements(array, c_array, JNI_ABORT);
  return v;
}

FOREVER::Variant JFloatArrayToVariant(JNIEnv* env, jfloatArray array) {
  const size_t len = env->GetArrayLength(array);
  jfloat* c_array = env->GetFloatArrayElements(array, nullptr);
  auto vec = new std::vector<FOREVER::Variant>(len);
  for (size_t i = 0; i < len; ++i) {
    (*vec)[i] = FOREVER::Variant(static_cast<float>(c_array[i]));
  }
  FOREVER::Variant v;
  v.AssignVector(&vec);
  env->ReleaseFloatArrayElements(array, c_array, JNI_ABORT);
  return v;
}

FOREVER::Variant JDoubleArrayToVariant(JNIEnv* env, jdoubleArray array) {
  const size_t len = env->GetArrayLength(array);
  jdouble* c_array = env->GetDoubleArrayElements(array, nullptr);
  auto vec = new std::vector<FOREVER::Variant>(len);
  for (size_t i = 0; i < len; ++i) {
    (*vec)[i] = FOREVER::Variant(static_cast<double>(c_array[i]));
  }
  FOREVER::Variant v;
  v.AssignVector(&vec);
  env->ReleaseDoubleArrayElements(array, c_array, JNI_ABORT);
  return v;
}

// Allow macros to iterate through all the primitive JNI types.
/*#define FIREBASE_JNI_PRIMITIVE_TYPES(X)                       \
  X(boolean_class, "[Z", jboolean, Boolean, bool, Bool, "%d") \
  X(byte_class, "[B", jbyte, Byte, uint8_t, UInt8, "%d")      \
  X(character_class, "[C", jchar, Char, char, Char, "%c")     \
  X(short_class, "[S", jshort, Short, int16_t, Int16, "%d")   \
  X(integer_class, "[I", jint, Int, int, Int, "%d")           \
  X(long_class, "[J", jlong, Long, int64_t, Int64, "%ld")     \
  X(float_class, "[F", jfloat, Float, float, Float, "%f")     \
  X(double_class, "[D", jdouble, Double, double, Double, "%f")*/

bool IsJBooleanArray(JNIEnv* env, jobject obj) {
  jclass array_class = env->FindClass("[Z");
  const bool is_array = env->IsInstanceOf(obj, array_class);
  env->DeleteLocalRef(array_class);
  return is_array;
}

bool IsJByteArray(JNIEnv* env, jobject obj) {
  jclass array_class = env->FindClass("[B");
  const bool is_array = env->IsInstanceOf(obj, array_class);
  env->DeleteLocalRef(array_class);
  return is_array;
}

bool IsJCharArray(JNIEnv* env, jobject obj) {
  jclass array_class = env->FindClass("[C");
  const bool is_array = env->IsInstanceOf(obj, array_class);
  env->DeleteLocalRef(array_class);
  return is_array;
}

bool IsJShortArray(JNIEnv* env, jobject obj) {
  jclass array_class = env->FindClass("[S");
  const bool is_array = env->IsInstanceOf(obj, array_class);
  env->DeleteLocalRef(array_class);
  return is_array;
}

bool IsJIntArray(JNIEnv* env, jobject obj) {
  jclass array_class = env->FindClass("[I");
  const bool is_array = env->IsInstanceOf(obj, array_class);
  env->DeleteLocalRef(array_class);
  return is_array;
}

bool IsJLongArray(JNIEnv* env, jobject obj) {
  jclass array_class = env->FindClass("[J");
  const bool is_array = env->IsInstanceOf(obj, array_class);
  env->DeleteLocalRef(array_class);
  return is_array;
}

bool IsJFloatArray(JNIEnv* env, jobject obj) {
  jclass array_class = env->FindClass("[F");
  const bool is_array = env->IsInstanceOf(obj, array_class);
  env->DeleteLocalRef(array_class);
  return is_array;
}

bool IsJDoubleArray(JNIEnv* env, jobject obj) {
  jclass array_class = env->FindClass("[D");
  const bool is_array = env->IsInstanceOf(obj, array_class);
  env->DeleteLocalRef(array_class);
  return is_array;
}

// Convert a jbyteArray to a vector, releasing the reference to the
// jbyteArray.
std::vector<unsigned char> JniByteArrayToVector(JNIEnv* env, jobject array) {
  std::vector<unsigned char> value;
  auto byte_array = reinterpret_cast<jbyteArray>(array);
  jsize byte_array_length = env->GetArrayLength(byte_array);
  if (byte_array_length) {
    value.resize(byte_array_length);
    env->GetByteArrayRegion(byte_array, 0, byte_array_length,
                            reinterpret_cast<jbyte*>(&value[0]));
  }
  env->DeleteLocalRef(array);
  return value;
}

// Convert a byte buffer and size into a jbyteArray.
jbyteArray ByteBufferToJavaByteArray(JNIEnv* env, const uint8_t* data,
                                     size_t size) {
  jbyteArray output_array = env->NewByteArray(static_cast<int>(size));
  env->SetByteArrayRegion(output_array, 0, static_cast<int>(size),
                          reinterpret_cast<const jbyte*>(data));
  return output_array;
}

jobject VariantToJavaObject(JNIEnv* env, const FOREVER::Variant& variant) {
  if (variant.is_null()) {
    return nullptr;
  } else if (variant.is_int64()) {
    jclass long_class = env->FindClass("java/lang/Long");
    jmethodID long_class_constructor_method_id =
        env->GetMethodID(long_class, "<init>", "(J)V");
    return env->NewObject(long_class, long_class_constructor_method_id,
                          variant.int64_value());
  } else if (variant.is_double()) {
    jclass double_class = env->FindClass("java/lang/Double");
    jmethodID double_class_constructor_method_id =
        env->GetMethodID(double_class, "<init>", "(D)V");
    return env->NewObject(double_class, double_class_constructor_method_id,
                          variant.double_value());
  } else if (variant.is_bool()) {
    jclass boolean_class = env->FindClass("java/lang/Boolean");
    jmethodID boolean_class_constructor_method_id =
        env->GetMethodID(boolean_class, "<init>", "(Z)V");
    return env->NewObject(boolean_class, boolean_class_constructor_method_id,
                          variant.bool_value());
  } else if (variant.is_string()) {
    return env->NewStringUTF(variant.string_value());
  } else if (variant.is_blob()) {
    return static_cast<jobject>(ByteBufferToJavaByteArray(
        env, variant.blob_data(), variant.blob_size()));
  } else if (variant.is_vector()) {
    return VariantVectorToJavaList(env, variant.vector());
  } else if (variant.is_map()) {
    return VariantMapToJavaMap(env, variant.map());
  } else {
    // Unsupported type.
    assert("Variant cannot be converted to Java Object, returning null.");
    return nullptr;
  }
}

// Converts a `std::vector<Variant>` to a `java.util.List<Object>`.
// Returns a local ref to a List.
jobject VariantVectorToJavaList(
    JNIEnv* env, const std::vector<FOREVER::Variant>& variant_vector) {
  jclass array_list = env->FindClass("java/util/ArrayList");
  jmethodID array_list_constructor_method_id =
      env->GetMethodID(array_list, "<init>", "()V");
  jmethodID array_list_add_method_id =
      env->GetMethodID(array_list, "add", "(Ljava/lang/Object;)Z");
  jobject java_list =
      env->NewObject(array_list, array_list_constructor_method_id);
  jmethodID add_method = array_list_add_method_id;
  for (const auto & it : variant_vector) {
    jobject value = VariantToJavaObject(env, it);
    env->CallBooleanMethod(java_list, add_method, value);
    CheckAndClearJniExceptions(env);
    env->DeleteLocalRef(value);
  }
  return java_list;
}

// Converts a `std::map<Variant, Variant>` to a `java.util.Map<Object, Object>`.
// Returns a local ref to a Map.
jobject VariantMapToJavaMap(
    JNIEnv* env,
    const std::map<FOREVER::Variant, FOREVER::Variant>& variant_map) {
  jclass hash_map = env->FindClass("java/util/HashMap");
  jmethodID hash_map_constructor_method_id =
      env->GetMethodID(hash_map, "<init>", "()V");
  jmethodID hash_map_put_method_id = env->GetMethodID(
      hash_map, "put",
      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
  jobject java_map = env->NewObject(hash_map, hash_map_constructor_method_id);
  jmethodID put_method = hash_map_put_method_id;
  for (const auto & it : variant_map) {
    jobject key = VariantToJavaObject(env, it.first);
    jobject value = VariantToJavaObject(env, it.second);
    jobject previous = env->CallObjectMethod(java_map, put_method, key, value);
    CheckAndClearJniExceptions(env);
    if (previous) env->DeleteLocalRef(previous);
    env->DeleteLocalRef(value);
    env->DeleteLocalRef(key);
  }
  return java_map;
}

FOREVER::Variant JavaObjectToVariant(JNIEnv* env, jobject object) {
  if (object == nullptr) return FOREVER::Variant();
  jclass string_class = env->FindClass("java/lang/String");
  jclass data_class = env->FindClass("java/util/Date");
  jclass boolean_class = env->FindClass("java/lang/Boolean");
  jclass map = env->FindClass("java/util/Map");
  jclass list = env->FindClass("java/util/List");
  jclass byte_class = env->FindClass("java/lang/Byte");
  jclass char_class = env->FindClass("java/lang/Character");
  jclass short_class = env->FindClass("java/lang/Short");
  jclass int_class = env->FindClass("java/lang/Integer");
  jclass long_class = env->FindClass("java/lang/Long");
  jclass float_class = env->FindClass("java/lang/Float");
  jclass double_class = env->FindClass("java/lang/Double");
  // Convert strings.
  if (env->IsInstanceOf(object, string_class))
    return FOREVER::Variant(JStringToString(env, object));

  // Convert Dates to millis since epoch.
  if (env->IsInstanceOf(object, data_class)) {
    jmethodID data_class_get_time_method_id =
        env->GetMethodID(data_class, "getTime", "()J");
    auto getTime = data_class_get_time_method_id;
    jlong millis = env->CallLongMethod(object, getTime);
    CheckAndClearJniExceptions(env);
    return FOREVER::Variant(static_cast<int64_t>(millis));
  }

  // Convert other primitive types.
  // FIREBASE_JNI_PRIMITIVE_TYPES(FIREBASE_CONVERT_TO_VARIANT)
  // Code like this, but for all JNI primitive types.
  //
  //  if (env->IsInstanceOf(object, boolean_class::GetClass())) {  \
//    return Variant(JBooleanToBool(env, object));
  //  }
  /*#define FIREBASE_CONVERT_TO_VARIANT(type_class, array_format, jtype, jname,
    \
                                      ctype, cname, printf_type)              \
    if (env->IsInstanceOf(object, type_class::GetClass())) {                  \
      return Variant(J##jname##To##cname(env, object));                       \
    }*/
  if (env->IsInstanceOf(object, boolean_class)) {
    return FOREVER::Variant(JBooleanToBool(env, object));
  }

  if (env->IsInstanceOf(object, byte_class)) {
    return FOREVER::Variant(JByteToUInt8(env, object));
  }

  if (env->IsInstanceOf(object, char_class)) {
    return FOREVER::Variant(JCharToChar(env, object));
  }

  if (env->IsInstanceOf(object, short_class)) {
    return FOREVER::Variant(JShortToInt16(env, object));
  }

  if (env->IsInstanceOf(object, int_class)) {
    return FOREVER::Variant(JIntegerToInt(env, object));
  }

  if (env->IsInstanceOf(object, long_class)) {
    return FOREVER::Variant(JLongToInt64(env, object));
  }

  if (env->IsInstanceOf(object, float_class)) {
    return FOREVER::Variant(JFloatToFloat(env, object));
  }

  if (env->IsInstanceOf(object, double_class)) {
    return FOREVER::Variant(JDoubleToDouble(env, object));
  }

  // Convert maps.
  if (env->IsInstanceOf(object, map)) {
    FOREVER::Variant v;
    auto c_map = new std::map<FOREVER::Variant, FOREVER::Variant>();
    JavaMapToVariantMap(env, c_map, object);
    v.AssignMap(&c_map);
    return v;
  }

  // Convert lists.
  if (env->IsInstanceOf(object, list)) {
    FOREVER::Variant v;
    auto c_vector = new std::vector<FOREVER::Variant>();
    JavaListToVariantList(env, c_vector, object);
    v.AssignVector(&c_vector);
    return v;
  }

  // Convert arrays.
  if (IsJArray(env, object)) {
    return JArrayToVariant(env, reinterpret_cast<jarray>(object));
  }

  // Unsupported type.
  assert("Class %s cannot be converted to Variant, leaving empty.");
  JObjectClassName(env, object).c_str();
  return FOREVER::Variant();
}

// Converts a `java.util.List<java.lang.Object>` to a `std::vector<Variant>`.
void JavaListToVariantList(JNIEnv* env, std::vector<FOREVER::Variant>* to,
                           jobject from) {
  jclass array_list_class = env->FindClass("java/util/List");
  jmethodID list_size_method_id =
      env->GetMethodID(array_list_class, "size", "()I");
  jmethodID list_get_method_id =
      env->GetMethodID(array_list_class, "get", "(I)Ljava/lang/Object;");
  /*jmethodID list_set_method_id = env->GetMethodID(
      array_list_class, "set", "(ILjava/lang/Object;)Ljava/lang/Object;");*/
  int size = env->CallIntMethod(from, list_size_method_id);
  CheckAndClearJniExceptions(env);
  to->clear();
  to->reserve(size);

  for (int i = 0; i < size; i++) {
    // Object obj = from.get(i);
    jobject obj = env->CallObjectMethod(from, list_get_method_id, i);
    CheckAndClearJniExceptions(env);

    to->push_back(JavaObjectToVariant(env, obj));
    env->DeleteLocalRef(obj);
  }
}

FOREVER::Variant JArrayToVariant(JNIEnv* env, jarray array) {
  assert(IsJArray(env, array));

  // Check each array type in turn, and convert to it if the type matches.
  // FIREBASE_JNI_PRIMITIVE_TYPES(FIREBASE_CONVERT_TO_VARIANT_ARRAY)
  if (IsJBooleanArray(env, array)) {
    return JBooleanArrayToVariant(env, reinterpret_cast<jbooleanArray>(array));
  }

  if (IsJByteArray(env, array)) {
    return JByteArrayToVariant(env, reinterpret_cast<jbyteArray>(array));
  }

  if (IsJBooleanArray(env, array)) {
    return JBooleanArrayToVariant(env, reinterpret_cast<jbooleanArray>(array));
  }

  if (IsJCharArray(env, array)) {
    return JCharArrayToVariant(env, reinterpret_cast<jcharArray>(array));
  }

  if (IsJShortArray(env, array)) {
    return JShortArrayToVariant(env, reinterpret_cast<jshortArray>(array));
  }

  if (IsJIntArray(env, array)) {
    return JIntArrayToVariant(env, reinterpret_cast<jintArray>(array));
  }

  if (IsJLongArray(env, array)) {
    return JLongArrayToVariant(env, reinterpret_cast<jlongArray>(array));
  }

  if (IsJFloatArray(env, array)) {
    return JFloatArrayToVariant(env, reinterpret_cast<jfloatArray>(array));
  }

  if (IsJDoubleArray(env, array)) {
    return JDoubleArrayToVariant(env, reinterpret_cast<jdoubleArray>(array));
  }

  /*#define FIREBASE_CONVERT_TO_VARIANT_ARRAY(type_class, array_format, jtype, \
                                            jname, ctype, cname, printf_type) \
    if (IsJ##jname##Array(env, array)) {                                      \
      return J##jname##ArrayToVariant(env, static_cast<jtype##Array>(array)); \
    }*/

  // Must be an array of objects. Convert each object independently.
  return JObjectArrayToVariant(env, reinterpret_cast<jobjectArray>(array));
}

FOREVER::Variant JObjectArrayToVariant(JNIEnv* env, jobjectArray array) {
  const size_t len = env->GetArrayLength(array);
  auto vec = new std::vector<FOREVER::Variant>();
  vec->reserve(len);

  // Loop through array converted each object into a Variant.
  for (size_t i = 0; i < len; ++i) {
    jobject obj = env->GetObjectArrayElement(array, static_cast<int>(i));
    vec->push_back(JavaObjectToVariant(env, obj));
    env->DeleteLocalRef(obj);
  }

  // Move vec so that we don't have to make a copy of it.
  FOREVER::Variant v;
  v.AssignVector(&vec);
  return v;
}
