// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "jni_util.h"

// This is called by the VM when the shared library is first loaded.
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  // Initialize the Java VM.
  Forever::jni::InitJavaVM(vm);

  JNIEnv* env = Forever::jni::AttachCurrentThread();
  bool result = false;

  return JNI_VERSION_1_6;
}
