#ifndef QANDROIDJNIENVIRONMENT_H
#define QANDROIDJNIENVIRONMENT_H

#include <jni.h>
#include <qglobal.h>

#include <memory>

struct QJNIEnvironmentPrivate;

class QAndroidJniEnvironment {
 public:
  QAndroidJniEnvironment();
  ~QAndroidJniEnvironment();
  static JavaVM *javaVM();
  JNIEnv *operator->();
  operator JNIEnv *() const;

 private:
  Q_DISABLE_COPY(QAndroidJniEnvironment)
  std::unique_ptr<QJNIEnvironmentPrivate> d;
};

#endif  // QANDROIDJNIENVIRONMENT_H