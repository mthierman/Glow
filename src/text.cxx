// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/log.hxx>
#include <glow/math.hxx>
#include <glow/text.hxx>

#include <objbase.h>

#include <stdexcept>

#include <wil/win32_helpers.h>

namespace glow::text {
auto utf8_to_utf16(std::string_view input) -> std::wstring {
    std::wstring output;

    if (input.length() > 0) {
        int inputLength { glow::math::check_safe_size<int>(input.length()) };

        auto outputLength { ::MultiByteToWideChar(
            CP_UTF8, 0, input.data(), inputLength, nullptr, 0) };

        output.resize(outputLength);

        if (::MultiByteToWideChar(
                CP_UTF8, 0, input.data(), inputLength, output.data(), outputLength)
            == 0) {
            throw std::runtime_error(glow::log::get_last_error());
        }
    }

    return output;
}

auto utf16_to_utf8(std::wstring_view input) -> std::string {
    std::string output;

    if (input.length() > 0) {
        int inputLength { glow::math::check_safe_size<int>(input.length()) };

        auto outputLength { ::WideCharToMultiByte(CP_UTF8,
                                                  WC_NO_BEST_FIT_CHARS,
                                                  input.data(),
                                                  inputLength,
                                                  nullptr,
                                                  0,
                                                  nullptr,
                                                  nullptr) };

        output.resize(outputLength);

        if (::WideCharToMultiByte(CP_UTF8,
                                  WC_NO_BEST_FIT_CHARS,
                                  input.data(),
                                  inputLength,
                                  output.data(),
                                  outputLength,
                                  nullptr,
                                  nullptr)
            == 0) {
            throw std::runtime_error(glow::log::get_last_error());
        }
    }

    return output;
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
