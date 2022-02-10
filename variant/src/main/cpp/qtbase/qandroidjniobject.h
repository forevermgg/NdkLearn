#ifndef QANDROIDJNIOBJECT_H
#define QANDROIDJNIOBJECT_H
#include <jni.h>
#include <qglobal.h>
#include <string.h>

#include <memory>

class QJNIObjectPrivate;

class QAndroidJniObject {
 public:
  QAndroidJniObject();
  explicit QAndroidJniObject(const char *className);
  explicit QAndroidJniObject(const char *className, const char *sig, ...);
  explicit QAndroidJniObject(jclass clazz);
  explicit QAndroidJniObject(jclass clazz, const char *sig, ...);
  QAndroidJniObject(jobject obj);
  ~QAndroidJniObject();

  template <typename T>
  inline T object() const {
    return static_cast<T>(javaObject());
  }

  inline jobject object() const { return javaObject(); }

  template <typename T>
  T callMethod(const char *methodName) const;
  template <typename T>
  T callMethod(const char *methodName, const char *sig, ...) const;
  template <typename T>
  QAndroidJniObject callObjectMethod(const char *methodName) const;
  QAndroidJniObject callObjectMethod(const char *methodName, const char *sig,
                                     ...) const;

  template <typename T>
  static T callStaticMethod(const char *className, const char *methodName);
  template <typename T>
  static T callStaticMethod(const char *className, const char *methodName,
                            const char *sig, ...);
  template <typename T>
  static QAndroidJniObject callStaticObjectMethod(const char *className,
                                                  const char *methodName);
  static QAndroidJniObject callStaticObjectMethod(const char *className,
                                                  const char *methodName,
                                                  const char *sig, ...);
  template <typename T>
  static T callStaticMethod(jclass clazz, const char *methodName);
  template <typename T>
  static T callStaticMethod(jclass clazz, const char *methodName,
                            const char *sig, ...);
  template <typename T>
  static QAndroidJniObject callStaticObjectMethod(jclass clazz,
                                                  const char *methodName);
  static QAndroidJniObject callStaticObjectMethod(jclass clazz,
                                                  const char *methodName,
                                                  const char *sig, ...);
  template <typename T>
  T getField(const char *fieldName) const;
  template <typename T>
  QAndroidJniObject getObjectField(const char *fieldName) const;
  template <typename T>
  QAndroidJniObject getObjectField(const char *fieldName,
                                   const char *sig) const;
  QAndroidJniObject getObjectField(const char *fieldName,
                                   const char *sig) const;
  template <typename T>
  void setField(const char *fieldName, T value);
  template <typename T>
  void setField(const char *fieldName, const char *sig, T value);
  template <typename T>
  static QAndroidJniObject getStaticObjectField(const char *className,
                                                const char *fieldName);
  template <typename T>
  static QAndroidJniObject getStaticObjectField(const char *className,
                                                const char *fieldName,
                                                const char *sig);
  static QAndroidJniObject getStaticObjectField(const char *className,
                                                const char *fieldName,
                                                const char *sig);
  template <typename T>
  static T getStaticField(const char *className, const char *fieldName);
  template <typename T>
  static QAndroidJniObject getStaticObjectField(jclass clazz,
                                                const char *fieldName);
  template <typename T>
  static QAndroidJniObject getStaticObjectField(jclass clazz,
                                                const char *fieldName,
                                                const char *sig);
  static QAndroidJniObject getStaticObjectField(jclass clazz,
                                                const char *fieldName,
                                                const char *sig);
  template <typename T>
  static T getStaticField(jclass clazz, const char *fieldName);

  template <typename T>
  static void setStaticField(const char *className, const char *fieldName,
                             const char *sig, T value);
  template <typename T>
  static void setStaticField(const char *className, const char *fieldName,
                             T value);
  template <typename T>
  static void setStaticField(jclass clazz, const char *fieldName,
                             const char *sig, T value);
  template <typename T>
  static void setStaticField(jclass clazz, const char *fieldName, T value);

  static QAndroidJniObject fromString(const std::string &string);
  std::string toString() const;

  static bool isClassAvailable(const char *className);
  bool isValid() const;

  static QAndroidJniObject fromLocalRef(jobject obj);

  template <typename T>
  inline QAndroidJniObject &operator=(T o) {
    assign(static_cast<jobject>(o));
    return *this;
  }

 private:
  friend bool operator==(const QAndroidJniObject &, const QAndroidJniObject &);
  friend bool operator!=(const QAndroidJniObject &, const QAndroidJniObject &);
  template <typename T>
  friend bool operator!=(const QAndroidJniObject &, const QAndroidJniObject &);
  template <typename T>
  friend bool operator==(const QAndroidJniObject &, const QAndroidJniObject &);

  QAndroidJniObject(const QJNIObjectPrivate &o);

  void assign(jobject o);
  jobject javaObject() const;
  bool isSameObject(jobject obj) const;
  bool isSameObject(const QAndroidJniObject &obj) const;

  std::shared_ptr<QJNIObjectPrivate> d;
};

inline bool operator==(const QAndroidJniObject &obj1,
                       const QAndroidJniObject &obj2) {
  return obj1.isSameObject(obj2);
}

inline bool operator!=(const QAndroidJniObject &obj1,
                       const QAndroidJniObject &obj2) {
  return !obj1.isSameObject(obj2);
}

template <typename T>
inline bool operator==(const QAndroidJniObject &obj1,
                       const QAndroidJniObject &obj2) {
  return obj1.isSameObject(obj2.object());
}

template <typename T>
inline bool operator!=(const QAndroidJniObject &obj1,
                       const QAndroidJniObject &obj2) {
  return !obj1.isSameObject(obj2.object());
}

#endif  // QANDROIDJNIOBJECT_H
