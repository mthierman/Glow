// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/text.hxx>

#include <Windows.h>

#include <objbase.h>

#include <stdexcept>

#include <wil/win32_helpers.h>

#include <glow/log.hxx>
#include <glow/math.hxx>

namespace glow::text {
auto to_string(std::wstring_view input) -> std::string {
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

auto to_wstring(std::string_view input) -> std::wstring {
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

auto to_string(std::u8string_view input) -> std::string {
    return std::string(input.begin(), input.end());
}

auto to_wstring(std::u8string_view input) -> std::wstring { return to_wstring(to_string(input)); }
}; // namespace glow::text
