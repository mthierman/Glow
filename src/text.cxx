// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/text.hxx>

#include <Windows.h>

#include <stdexcept>

#include <wil/win32_helpers.h>

#include <glow/log.hxx>
#include <glow/math.hxx>

namespace glow::text {
auto wchar_to_string(const wchar_t* input, size_t length) -> std::string {
    std::string output;

    if (length > 0) {
        int inputLength { glow::math::check_safe_size<int>(length) };

        auto outputLength { ::WideCharToMultiByte(
            CP_UTF8, WC_NO_BEST_FIT_CHARS, input, inputLength, nullptr, 0, nullptr, nullptr) };

        output.resize(outputLength);

        if (::WideCharToMultiByte(CP_UTF8,
                                  WC_NO_BEST_FIT_CHARS,
                                  input,
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

auto wchar_to_u8string(const wchar_t* input, size_t length) -> std::u8string {
    std::u8string output;

    if (length > 0) {
        int inputLength { glow::math::check_safe_size<int>(length) };

        auto outputLength { ::WideCharToMultiByte(
            CP_UTF8, WC_NO_BEST_FIT_CHARS, input, inputLength, nullptr, 0, nullptr, nullptr) };

        output.resize(outputLength);

        if (::WideCharToMultiByte(CP_UTF8,
                                  WC_NO_BEST_FIT_CHARS,
                                  input,
                                  inputLength,
                                  reinterpret_cast<char*>(output.data()),
                                  outputLength,
                                  nullptr,
                                  nullptr)
            == 0) {
            throw std::runtime_error(glow::log::get_last_error());
        }
    }

    return output;
}

auto char_to_wstring(const char* input, size_t length) -> std::wstring {
    std::wstring output;

    if (length > 0) {
        int inputLength { glow::math::check_safe_size<int>(length) };

        auto outputLength { ::MultiByteToWideChar(CP_UTF8, 0, input, inputLength, nullptr, 0) };

        output.resize(outputLength);

        if (::MultiByteToWideChar(CP_UTF8, 0, input, inputLength, output.data(), outputLength)
            == 0) {
            throw std::runtime_error(glow::log::get_last_error());
        }
    }

    return output;
}

auto char_to_u16string(const char* input, size_t length) -> std::u16string {
    std::u16string output;

    if (length > 0) {
        int inputLength { glow::math::check_safe_size<int>(length) };

        auto outputLength { ::MultiByteToWideChar(CP_UTF8, 0, input, inputLength, nullptr, 0) };

        output.resize(outputLength);

        if (::MultiByteToWideChar(CP_UTF8,
                                  0,
                                  input,
                                  inputLength,
                                  reinterpret_cast<wchar_t*>(output.data()),
                                  outputLength)
            == 0) {
            throw std::runtime_error(glow::log::get_last_error());
        }
    }

    return output;
}

// to_string & to_wstring

auto to_string(std::string_view input) -> std::string { return input.data(); }

auto to_wstring(std::wstring_view input) -> std::wstring { return input.data(); }

auto to_string(std::wstring_view input) -> std::string {
    return wchar_to_string(input.data(), input.length());
}

auto to_wstring(std::string_view input) -> std::wstring {
    return char_to_wstring(input.data(), input.length());
}

auto to_string(std::u8string_view input) -> std::string {
    return std::string(input.begin(), input.end());
}

auto to_wstring(std::u16string_view input) -> std::wstring {
    return std::wstring(input.begin(), input.end());
}

auto to_string(std::u16string_view input) -> std::string {
    return wchar_to_string(reinterpret_cast<const wchar_t*>(input.data()), input.length());
}

auto to_wstring(std::u8string_view input) -> std::wstring {
    return char_to_wstring(reinterpret_cast<const char*>(input.data()), input.length());
}

// u8string & u16string

auto to_u8string(std::u8string_view input) -> std::u8string { return input.data(); }

auto to_u8string(std::string_view input) -> std::u8string {
    return std::u8string(input.begin(), input.end());
}

auto to_u16string(std::u16string_view input) -> std::u16string { return input.data(); }

auto to_u16string(std::wstring_view input) -> std::u16string {
    return std::u16string(input.begin(), input.end());
}

// u8string & u16string convert

auto to_u8string(std::wstring_view input) -> std::u8string {
    return wchar_to_u8string(input.data(), input.length());
}

auto to_u8string(std::u16string_view input) -> std::u8string {
    return wchar_to_u8string(reinterpret_cast<const wchar_t*>(input.data()), input.length());
}

auto to_u16string(std::string_view input) -> std::u16string {
    return char_to_u16string(input.data(), input.length());
}

auto to_u16string(std::u8string_view input) -> std::u16string {
    return char_to_u16string(reinterpret_cast<const char*>(input.data()), input.length());
}

// String
String::String(std::u8string string)
    : storage { string } { }

String::String(std::u8string&& string)
    : storage { std::move(string) } { }

auto String::operator()(std::u8string string) -> std::u8string& {
    storage = std::move(string);

    return storage;
}
}; // namespace glow::text
