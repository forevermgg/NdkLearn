#ifndef QJNI_P_H
#define QJNI_P_H

#include <jni.h>
#include <memory>
#include "qglobal.h"

struct QJNILocalRefDeleter {
  static void cleanup(jobject obj);
};

// To simplify this we only define it for jobjects.
typedef std::unique_ptr<_jobject, QJNILocalRefDeleter> QJNIScopedLocalRef;

class QJNIEnvironmentPrivate {
 public:
  QJNIEnvironmentPrivate();
  ~QJNIEnvironmentPrivate();
  JNIEnv *operator->();
  operator JNIEnv *() const;
  static jclass findClass(const char *className, JNIEnv *env = 0);

 private:
  friend class QAndroidJniEnvironment;
  Q_DISABLE_COPY(QJNIEnvironmentPrivate)
  JNIEnv *jniEnv;
};

class QJNIObjectData {
 public:
  QJNIObjectData();
  ~QJNIObjectData();
  jobject m_jobject;
  jclass m_jclass;
  bool m_own_jclass;
  char *m_className;
};

class QJNIObjectPrivate {
 public:
  QJNIObjectPrivate();
  explicit QJNIObjectPrivate(const char *className);
  QJNIObjectPrivate(const char *className, const char *sig, ...);
  explicit QJNIObjectPrivate(jclass clazz);
  QJNIObjectPrivate(jclass clazz, const char *sig, ...);
  // In most cases you should never call this function with a local ref. unless
  // you intend to manage the local ref. yourself. NOTE: see fromLocalRef() for
  // converting a local ref. to QJNIObjectPrivate.
  explicit QJNIObjectPrivate(jobject globalRef);

  template <typename T>
  T callMethod(const char *methodName, const char *sig, ...) const;
  template <typename T>
  T callMethod(const char *methodName) const;
  template <typename T>
  QJNIObjectPrivate callObjectMethod(const char *methodName) const;
  QJNIObjectPrivate callObjectMethod(const char *methodName, const char *sig,
                                     ...) const;
  template <typename T>
  static T callStaticMethod(const char *className, const char *methodName,
                            const char *sig, ...);
  template <typename T>
  static T callStaticMethod(const char *className, const char *methodName);
  template <typename T>
  static T callStaticMethod(jclass clazz, const char *methodName,
                            const char *sig, ...);
  template <typename T>
  static T callStaticMethod(jclass clazz, const char *methodName);
  static QJNIObjectPrivate callStaticObjectMethod(const char *className,
                                                  const char *methodName,
                                                  const char *sig, ...);

  static QJNIObjectPrivate callStaticObjectMethod(jclass clazz,
                                                  const char *methodName,
                                                  const char *sig, ...);

  template <typename T>
  T getField(const char *fieldName) const;
  template <typename T>
  static T getStaticField(const char *className, const char *fieldName);
  template <typename T>
  static T getStaticField(jclass clazz, const char *fieldName);

  QJNIObjectPrivate getObjectField(const char *fieldName,
                                   const char *sig) const;
  static QJNIObjectPrivate getStaticObjectField(const char *className,
                                                const char *fieldName,
                                                const char *sig);
  static QJNIObjectPrivate getStaticObjectField(jclass clazz,
                                                const char *fieldName,
                                                const char *sig);

  template <typename T>
  void setField(const char *fieldName, T value);
  template <typename T>
  void setField(const char *fieldName, const char *sig, T value);
  template <typename T>
  static void setStaticField(const char *className, const char *fieldName,
                             T value);
  template <typename T>
  static void setStaticField(const char *className, const char *fieldName,
                             const char *sig, T value);
  template <typename T>
  static void setStaticField(jclass clazz, const char *fieldName,
                             const char *sig, T value);

  template <typename T>
  static void setStaticField(jclass clazz, const char *fieldName, T value);

  static QJNIObjectPrivate fromString(const std::string& string);
  std::string toString() const;

  static bool isClassAvailable(const char *className);
  bool isValid() const;
  jobject object() const { return d->m_jobject; }

  template <typename T>
  inline QJNIObjectPrivate &operator=(T o) {
    jobject jobj = static_cast<jobject>(o);
    if (!isSameObject(jobj)) {
      d = std::shared_ptr<QJNIObjectData>(new QJNIObjectData());
      if (jobj) {
        QJNIEnvironmentPrivate env;
        d->m_jobject = env->NewGlobalRef(jobj);
        jclass objectClass = env->GetObjectClass(jobj);
        d->m_jclass = static_cast<jclass>(env->NewGlobalRef(objectClass));
        env->DeleteLocalRef(objectClass);
      }
    }

    return *this;
  }

  // This function takes ownership of the jobject and releases the local ref.
  // before returning.
  static QJNIObjectPrivate fromLocalRef(jobject lref);

 private:
  friend class QAndroidJniObject;

  struct QVaListPrivate {
    operator va_list &() const { return m_args; }
    va_list &m_args;
  };

  QJNIObjectPrivate(const char *className, const char *sig,
                    const QVaListPrivate &args);
  QJNIObjectPrivate(jclass clazz, const char *sig, const QVaListPrivate &args);

  template <typename T>
  T callMethodV(const char *methodName, const char *sig, va_list args) const;
  QJNIObjectPrivate callObjectMethodV(const char *methodName, const char *sig,
                                      va_list args) const;
  template <typename T>
  static T callStaticMethodV(const char *className, const char *methodName,
                             const char *sig, va_list args);
  template <typename T>
  static T callStaticMethodV(jclass clazz, const char *methodName,
                             const char *sig, va_list args);
  static QJNIObjectPrivate callStaticObjectMethodV(const char *className,
                                                   const char *methodName,
                                                   const char *sig,
                                                   va_list args);

  static QJNIObjectPrivate callStaticObjectMethodV(jclass clazz,
                                                   const char *methodName,
                                                   const char *sig,
                                                   va_list args);

  bool isSameObject(jobject obj) const;
  bool isSameObject(const QJNIObjectPrivate &other) const;

  friend bool operator==(const QJNIObjectPrivate &, const QJNIObjectPrivate &);
  friend bool operator!=(const QJNIObjectPrivate &, const QJNIObjectPrivate &);
  template <typename T>
  friend bool operator!=(const QJNIObjectPrivate &, T);
  template <typename T>
  friend bool operator==(const QJNIObjectPrivate &, T);
  template <typename T>
  friend bool operator!=(T, const QJNIObjectPrivate &);
  template <typename T>
  friend bool operator==(T, const QJNIObjectPrivate &);

  std::shared_ptr<QJNIObjectData> d;
};

inline bool operator==(const QJNIObjectPrivate &obj1,
                       const QJNIObjectPrivate &obj2) {
  return obj1.isSameObject(obj2);
}

inline bool operator!=(const QJNIObjectPrivate &obj1,
                       const QJNIObjectPrivate &obj2) {
  return !obj1.isSameObject(obj2);
}

template <typename T>
inline bool operator==(const QJNIObjectPrivate &obj1, T obj2) {
  return obj1.isSameObject(static_cast<jobject>(obj2));
}

template <typename T>
inline bool operator==(T obj1, const QJNIObjectPrivate &obj2) {
  return obj2.isSameObject(static_cast<jobject>(obj1));
}

template <typename T>
inline bool operator!=(const QJNIObjectPrivate &obj1, T obj2) {
  return !obj1.isSameObject(obj2);
}

template <typename T>
inline bool operator!=(T obj1, const QJNIObjectPrivate &obj2) {
  return !obj2.isSameObject(obj1);
}

#endif  // QJNI_P_H
