#include "qandroidjnienvironment.h"

#include <qjni_p.h>
#include <qjnihelpers_p.h>

QAndroidJniEnvironment::QAndroidJniEnvironment()
    : d(new QJNIEnvironmentPrivate) {}

QAndroidJniEnvironment::~QAndroidJniEnvironment() {}

JavaVM *QAndroidJniEnvironment::javaVM() { return QtAndroidPrivate::javaVM(); }

JNIEnv *QAndroidJniEnvironment::operator->() { return d->jniEnv; }

QAndroidJniEnvironment::operator JNIEnv *() const { return d->jniEnv; }
