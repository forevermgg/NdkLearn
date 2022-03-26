// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "logging.h"

#include <algorithm>
#include <iostream>

#include "build_config.h"
#include "log_settings.h"

#if defined(FML_OS_ANDROID)
#include <android/log.h>
#elif defined(FML_OS_IOS)
#include <syslog.h>
#elif defined(OS_FUCHSIA)
#include <lib/syslog/global.h>
#endif

namespace Forever {

namespace {

#if !defined(OS_FUCHSIA)
const char* const kLogSeverityNames[LOG_NUM_SEVERITIES] = {"INFO", "WARNING",
                                                           "ERROR", "FATAL"};

const char* GetNameForLogSeverity(LogSeverity severity) {
  if (severity >= LOG_INFO && severity < LOG_NUM_SEVERITIES) {
    return kLogSeverityNames[severity];
  }
  return "UNKNOWN";
}

const char* StripDots(const char* path) {
  while (strncmp(path, "../", 3) == 0) {
    path += 3;
  }
  return path;
}

const char* StripPath(const char* path) {
  auto* p = strrchr(path, '/');
  if (p) {
    return p + 1;
  }
  return path;
}
#endif

}  // namespace

LogMessage::LogMessage(LogSeverity severity, const char* file, int line, const char* function,
                       const char* condition)
    : severity_(severity), file_(file), line_(line), function_(function) {
#if !defined(OS_FUCHSIA)
  stream_ << "[";
  if (severity >= LOG_INFO) {
    stream_ << GetNameForLogSeverity(severity);
  } else {
    stream_ << "VERBOSE" << -severity;
  }
  stream_ << ":" << (severity > LOG_INFO ? StripDots(file_) : StripPath(file_))
          << "(" << line_ << ")] " << function_ << " ";
#endif

  if (condition) {
    stream_ << "Check failed: " << condition << ". ";
  }
}

LogMessage::~LogMessage() {
#if !defined(OS_FUCHSIA)
  stream_ << std::endl;
#endif

#if defined(FML_OS_ANDROID)
  android_LogPriority priority =
      (severity_ < 0) ? ANDROID_LOG_VERBOSE : ANDROID_LOG_UNKNOWN;
  switch (severity_) {
    case LOG_INFO:
      priority = ANDROID_LOG_INFO;
      break;
    case LOG_WARNING:
      priority = ANDROID_LOG_WARN;
      break;
    case LOG_ERROR:
      priority = ANDROID_LOG_ERROR;
      break;
    case LOG_FATAL:
      priority = ANDROID_LOG_FATAL;
      break;
  }
  __android_log_write(priority, "Forever", stream_.str().c_str());
#elif defined(FML_OS_IOS)
  syslog(LOG_ALERT, "%s", stream_.str().c_str());
#elif defined(OS_FUCHSIA)
  fx_log_severity_t fx_severity;
  switch (severity_) {
    case LOG_INFO:
      fx_severity = FX_LOG_INFO;
      break;
    case LOG_WARNING:
      fx_severity = FX_LOG_WARNING;
      break;
    case LOG_ERROR:
      fx_severity = FX_LOG_ERROR;
      break;
    case LOG_FATAL:
      fx_severity = FX_LOG_FATAL;
      break;
    default:
      if (severity_ < 0) {
        fx_severity = fx_log_severity_from_verbosity(-severity_);
      } else {
        // Unknown severity. Use INFO.
        fx_severity = FX_LOG_INFO;
      }
  }
  fx_logger_log_with_source(fx_log_get_logger(), fx_severity, nullptr, file_,
                            line_, stream_.str().c_str());
#else
  std::cerr << stream_.str();
  std::cerr.flush();
#endif

  if (severity_ >= LOG_FATAL) {
    KillProcess();
  }
}

int GetVlogVerbosity() { return std::max(-1, LOG_INFO - GetMinLogLevel()); }

bool ShouldCreateLogMessage(LogSeverity severity) {
  return severity >= GetMinLogLevel();
}

void KillProcess() { abort(); }

}  // namespace Forever
