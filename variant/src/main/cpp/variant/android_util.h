#include <jni.h>

#include <map>
#include <string>
#include <vector>

#include "variant.h"

// Check for JNI exceptions, report them if any, and clear them. Returns true
// if there was an exception, false if not.
bool CheckAndClearJniExceptions(JNIEnv* env);

// Returns true of `obj` is a Java array.
bool IsJArray(JNIEnv* env, jobject obj);

// Return the name of the class of obj.
std::string JObjectClassName(JNIEnv* env, jobject obj);

// Converts a `std::vector<std::string>` to a `java.util.ArrayList<String>`
// Returns a local ref to a List.
jobject StdVectorToJavaList(JNIEnv* env,
                            const std::vector<std::string>& string_vector);

// Converts a `std::vector<std::string>` to a `java.util.ArrayList<String>`
// Returns a local ref to a List.
jobject StdVectorToJavaList(JNIEnv* env,
                            const std::vector<std::string>& string_vector);

// Converts an `std::map<const char*, const char*>` to a
// `java.util.Map<String, String>`.
void StdMapToJavaMap(JNIEnv* env, jobject* to,
                     const std::map<const char*, const char*>& string_map);

// Converts an `std::map<std::string, std::string>` to a
// `java.util.Map<String, String>`.
void StdMapToJavaMap(JNIEnv* env, jobject* to,
                     const std::map<std::string, std::string>& from);

// Converts a `java.util.Map<String, String>` to an
// `std::map<std::string, std::string>`.
void JavaMapToStdMap(JNIEnv* env, std::map<std::string, std::string>* to,
                     jobject from);

// Converts a `java.util.Map<java.lang.Object, java.lang.Object>` to an
// `std::map<Variant, Variant>`.
void JavaMapToVariantMap(JNIEnv* env,
                         std::map<FOREVER::Variant, FOREVER::Variant>* to,
                         jobject from);

// Converts a `java.util.Set<String>` to a `std::vector<std::string>`.
void JavaSetToStdStringVector(JNIEnv* env, std::vector<std::string>* to,
                              jobject from);

// Converts a `java.util.List<String>` to a `std::vector<std::string>`.
void JavaListToStdStringVector(JNIEnv* env, std::vector<std::string>* to,
                               jobject from);

// Converts a `java.util.List<Object>` to a `std::vector<std::string>`,
// calling toString() on each object first.
void JavaObjectListToStdStringVector(JNIEnv* env, std::vector<std::string>* to,
                                     jobject from);

// Convert a jstring to a std::string, releasing the reference to the jstring.
std::string JniStringToString(JNIEnv* env, jobject string_object);

// Convert a Java object of type java.lang.Object into an std::string by
// calling toString(), then release the object.
std::string JniObjectToString(JNIEnv* env, jobject obj);

// Convert a jstring (created by the JVM e.g passed into a native method) into
// a std::string.  Unlike JniStringToString() this does not release the
// reference to the string_object as the caller owns the object in a native
// method.
std::string JStringToString(JNIEnv* env, jobject string_object);

// Convert a Java Boolean object into a C++ bool.
// Note that a Java Boolean object is different from a jboolean primitive type.
bool JBooleanToBool(JNIEnv* env, jobject obj);

// Convert a Java Byte object into a C++ uint8_t.
// Note that a Java Byte object is different from a jbyte primitive type.
uint8_t JByteToUInt8(JNIEnv* env, jobject obj);

// Convert a Java Character object into a C++ char.
// Note that a Java Char object is different from a jchar primitive type.
char JCharToChar(JNIEnv* env, jobject obj);

// Convert a Java Short object into a C++ int16_t.
// Note that a Java Short object is different from a jshort primitive type.
int16_t JShortToInt16(JNIEnv* env, jobject obj);

// Convert a Java Integer object into a C++ int.
// Note that a Java Integer object is different from a jint primitive type.
int JIntegerToInt(JNIEnv* env, jobject obj);

// Convert a Java Long object into a C++ int64_t.
// Note that a Java Long object is different from a jlong primitive type.
int64_t JLongToInt64(JNIEnv* env, jobject obj);

// Convert a Java Float object into a C++ float.
// Note that a Java Float object is different from a jfloat primitive type.
float JFloatToFloat(JNIEnv* env, jobject obj);

// Convert a Java Double object into a C++ double.
// Note that a Java Double object is different from a jdouble primitive type.
double JDoubleToDouble(JNIEnv* env, jobject obj);

// Convert a Java array of type jboolean into a Variant that holds a vector.
FOREVER::Variant JBooleanArrayToVariant(JNIEnv* env, jbooleanArray array);

// Convert a Java array of type jbyte into a Variant that holds a vector.
FOREVER::Variant JByteArrayToVariant(JNIEnv* env, jbyteArray array);

// Convert a Java array of type jchar into a Variant that holds a vector.
FOREVER::Variant JCharArrayToVariant(JNIEnv* env, jcharArray array);

// Convert a Java array of type jshort into a Variant that holds a vector.
FOREVER::Variant JShortArrayToVariant(JNIEnv* env, jshortArray array);

// Convert a Java array of type jint into a Variant that holds a vector.
FOREVER::Variant JIntArrayToVariant(JNIEnv* env, jintArray array);

// Convert a Java array of type jlong into a Variant that holds a vector.
FOREVER::Variant JLongArrayToVariant(JNIEnv* env, jlongArray array);

// Convert a Java array of type jfloat into a Variant that holds a vector.
FOREVER::Variant JFloatArrayToVariant(JNIEnv* env, jfloatArray array);

// Convert a Java array of type jdouble into a Variant that holds a vector.
FOREVER::Variant JDoubleArrayToVariant(JNIEnv* env, jdoubleArray array);

bool IsJBooleanArray(JNIEnv* env, jobject obj);
bool IsJByteArray(JNIEnv* env, jobject obj);
bool IsJCharArray(JNIEnv* env, jobject obj);
bool IsJShortArray(JNIEnv* env, jobject obj);
bool IsJIntArray(JNIEnv* env, jobject obj);
bool IsJLongArray(JNIEnv* env, jobject obj);
bool IsJFloatArray(JNIEnv* env, jobject obj);
bool IsJDoubleArray(JNIEnv* env, jobject obj);

// Convert a jbyteArray to a vector, releasing the reference to the jbyteArray.
std::vector<unsigned char> JniByteArrayToVector(JNIEnv* env, jobject array);

// Convert a byte array with size into a Java byte[] (jbyteArray in JNI).
// The caller must call env->DeleteLocalRef() on the returned object.
jbyteArray ByteBufferToJavaByteArray(JNIEnv* env, const uint8_t* data,
                                     size_t size);

// Convert our Variant class into a generic Java object.
// Can be recursive. That is, Variant might be a map of Variants, which are
// vector of Variants, etc.
jobject VariantToJavaObject(JNIEnv* env, const FOREVER::Variant& variant);
jobject VariantVectorToJavaList(
    JNIEnv* env, const std::vector<FOREVER::Variant>& variant_vector);
jobject VariantMapToJavaMap(
    JNIEnv* env,
    const std::map<FOREVER::Variant, FOREVER::Variant>& variant_map);
// Convert a generic Java object into our Variant class.
// Can be recursive. That is, Variant might be a map of Variants, which are
// array of Variants, etc.
FOREVER::Variant JavaObjectToVariant(JNIEnv* env, jobject object);
void JavaListToVariantList(JNIEnv* env, std::vector<FOREVER::Variant>* to,
                           jobject from);
FOREVER::Variant JArrayToVariant(JNIEnv* env, jarray array);
// Convert a Java array of objects into a Variant that holds a vector of
// whatever those objects are. Assumes that the objects are of a type that
// a variant can hold (i.e. trees of dictionaries and arrays whose leaves are
// all primitive types).
FOREVER::Variant JObjectArrayToVariant(JNIEnv* env, jobjectArray array);


static inline bool to_bool(jboolean b) {
  return b == JNI_TRUE;
}

static inline jboolean to_jbool(bool b) {
  return b ? JNI_TRUE : JNI_FALSE;
}