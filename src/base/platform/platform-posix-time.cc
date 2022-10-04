// Copyright 2017 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <cmath>

#include "src/base/platform/platform-posix-time.h"

namespace v8 {
namespace base {

const char* PosixDefaultTimezoneCache::LocalTimezone(double time) {
  if (std::isnan(time)) return "";
  time_t tv = static_cast<time_t>(std::floor(time / msPerSecond));
  struct tm tm;
  struct tm* t = localtime_r(&tv, &tm);
  if (!t || !t->tm_zone) return "";
  return t->tm_zone;
}

double PosixDefaultTimezoneCache::LocalTimeOffset(double time_ms, bool is_utc) {
  // Preserve the old behavior for non-ICU implementation by ignoring both
  // time_ms and is_utc.
  tzset();
  return -static_cast<double>(timezone * msPerSecond);
}

double PosixDefaultTimezoneCache::DaylightSavingsOffset(double time) {
  if (std::isnan(time)) return std::numeric_limits<double>::quiet_NaN();
  time_t tv = static_cast<time_t>(std::floor(time / msPerSecond));
  struct tm tm;
  struct tm* t = localtime_r(&tv, &tm);
  if (nullptr == t) return std::numeric_limits<double>::quiet_NaN();
  // tm_gmtoff includes any daylight savings offset,
  // so subtract local time offset to get it.
  // Arguments of LocalTimeOffset() are ignored.
  return static_cast<double>(t->tm_gmtoff * msPerSecond) -
         LocalTimeOffset(time, true);
}

}  // namespace base
}  // namespace v8
