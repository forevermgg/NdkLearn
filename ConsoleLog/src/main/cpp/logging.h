// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FOREVER_FML_LOGGING_H_
#define FOREVER_FML_LOGGING_H_

#include <sstream>

#include "log_level.h"
#include "macros.h"

namespace Forever {

class LogMessageVoidify {
 public:
  void operator&(std::ostream&) {}
};

class LogMessage {
 public:
  LogMessage(LogSeverity severity, const char* file, int line,
             const char* function,
             const char* condition);
  ~LogMessage();

  std::ostream& stream() { return stream_; }

 private:
  std::ostringstream stream_;
  const LogSeverity severity_;
  const char* file_;
  const char* function_;
  const int line_;

  FML_DISALLOW_COPY_AND_ASSIGN(LogMessage);
};

// Gets the FML_VLOG default verbosity level.
int GetVlogVerbosity();

// Returns true if |severity| is at or above the current minimum log level.
// LOG_FATAL and above is always true.
bool ShouldCreateLogMessage(LogSeverity severity);

[[noreturn]] void KillProcess();

}  // namespace Forever

#define FML_LOG_STREAM(severity)                                       \
  ::Forever::LogMessage(::Forever::LOG_##severity, __FILE__, __LINE__, \
                        __FUNCTION__, nullptr)                         \
      .stream()

#define FML_LAZY_STREAM(stream, condition) \
  !(condition) ? (void)0 : ::Forever::LogMessageVoidify() & (stream)

#define FML_EAT_STREAM_PARAMETERS(ignored)                             \
  true || (ignored)                                                    \
      ? (void)0                                                        \
      : ::Forever::LogMessageVoidify() &                               \
        ::Forever::LogMessage(::Forever::LOG_FATAL, 0, 0, "", nullptr) \
                .stream()

#define FML_LOG_IS_ON(severity) \
  (::Forever::ShouldCreateLogMessage(::Forever::LOG_##severity))

#define FML_LOG(severity) \
  FML_LAZY_STREAM(FML_LOG_STREAM(severity), FML_LOG_IS_ON(severity))

#define FML_CHECK(condition)                                            \
  FML_LAZY_STREAM(::Forever::LogMessage(::Forever::LOG_FATAL, __FILE__, \
                                        __LINE__,                       \
                                        __FUNCTION__, #condition)       \
                      .stream(),                                        \
                  !(condition))

#define FML_VLOG_IS_ON(verbose_level) \
  ((verbose_level) <= ::Forever::GetVlogVerbosity())

// The VLOG macros log with negative verbosities.
#define FML_VLOG_STREAM(verbose_level) \
  ::Forever::LogMessage(-verbose_level, __FILE__, __LINE__, __FUNCTION__, nullptr).stream()

#define FML_VLOG(verbose_level) \
  FML_LAZY_STREAM(FML_VLOG_STREAM(verbose_level), FML_VLOG_IS_ON(verbose_level))

#ifndef NDEBUG
#define FML_DLOG(severity) FML_LOG(severity)
#define FML_DCHECK(condition) FML_CHECK(condition)
#else
#define FML_DLOG(severity) FML_EAT_STREAM_PARAMETERS(true)
#define FML_DCHECK(condition) FML_EAT_STREAM_PARAMETERS(condition)
#endif

#define FML_UNREACHABLE()                          \
  {                                                \
    FML_LOG(ERROR) << "Reached unreachable code."; \
    ::Forever::KillProcess();                      \
  }

#endif  // FOREVER_FML_LOGGING_H_
