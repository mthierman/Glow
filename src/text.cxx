// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/math.hxx>
#include <glow/text.hxx>

#include <objbase.h>

#include <stdexcept>

#include <wil/win32_helpers.h>

namespace glow::text {
auto utf8_to_utf16(std::string_view utf8) -> std::wstring {
    std::wstring utf16;

    if (utf8.length() > 0) {
        int safeSize { glow::math::check_safe_size<int>(utf8.length()) };

        auto length { ::MultiByteToWideChar(
            CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), safeSize, nullptr, 0) };

        utf16.resize(length);

        if (::MultiByteToWideChar(
                CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), safeSize, utf16.data(), length)
            == 0) {
            throw std::exception("UTF8 to UTF16 conversion failed");
        }
    }

    return utf16;
}

auto utf16_to_utf8(std::wstring_view utf16) -> std::string {
    std::string utf8;

    if (utf16.length() > 0) {
        int safeSize { glow::math::check_safe_size<int>(utf16.length()) };

        auto length { ::WideCharToMultiByte(CP_UTF8,
                                            WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                            utf16.data(),
                                            safeSize,
                                            nullptr,
                                            0,
                                            nullptr,
                                            nullptr) };

        utf8.resize(length);

        if (::WideCharToMultiByte(CP_UTF8,
                                  WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                  utf16.data(),
                                  safeSize,
                                  utf8.data(),
                                  length,
                                  nullptr,
                                  nullptr)
            == 0) {
            throw std::exception("UTF16 to UTF8 conversion failed");
        }
    }

    return utf8;
}

auto guid_to_wstring(const ::GUID& guid) -> std::wstring {
    wchar_t buffer[wil::guid_string_buffer_length];

    if (::StringFromGUID2(guid, buffer, wil::guid_string_buffer_length) == 0) {
        throw std::overflow_error("Buffer is too small to contain the string");
    }

    return buffer;
}

auto guid_to_string(const ::GUID& guid) -> std::string {
    return utf16_to_utf8(guid_to_wstring(guid));
}
}; // namespace glow::text
