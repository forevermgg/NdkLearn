// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "string_conversion.h"

#include <codecvt>
#include <locale>
#include <string>

#include "build_config.h"

#if defined(FML_OS_WIN)
// TODO(naifu): https://github.com/flutter/flutter/issues/98074
// Eliminate this workaround for a link error on Windows when the underlying
// bug is fixed.
std::locale::id std::codecvt<char16_t, char, _Mbstatet>::id;
#endif  // defined(FML_OS_WIN)

namespace Forever {

using Utf16StringConverter =
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>;

std::string Utf16ToUtf8(const std::u16string_view string) {
  Utf16StringConverter converter;
  return converter.to_bytes(string.data());
}

std::u16string Utf8ToUtf16(const std::string_view string) {
  Utf16StringConverter converter;
  return converter.from_bytes(string.data());
}

}  // namespace fml
