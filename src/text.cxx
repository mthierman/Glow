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
auto convert(std::u8string_view input) -> std::optional<std::u16string> {
    auto buffer { std::string(input.begin(), input.end()) };

    if (auto length { buffer.length() }; length > 0) {
        if (length > std::numeric_limits<int>::max()) {
            return std::nullopt;
        }

        auto outputLength { ::MultiByteToWideChar(
            CP_UTF8, 0, buffer.c_str(), static_cast<int>(length), nullptr, 0) };

        std::u16string output;
        output.resize(outputLength);

        if (::MultiByteToWideChar(CP_UTF8,
                                  0,
                                  buffer.c_str(),
                                  static_cast<int>(length),
                                  reinterpret_cast<wchar_t*>(output.data()),
                                  outputLength)
            != 0) {
            return output;
        } else {
            return std::nullopt;
        }
    } else {
        return std::nullopt;
    }
}

auto wchar_to_string(const wchar_t* input, size_t length) -> std::string {
    std::string output;

    if (length > 0) {
        int inputLength { glow::math::check_safe_size<int>(length) };

        auto outputLength { ::WideCharToMultiByte(
            CP_UTF8, 0, input, inputLength, nullptr, 0, nullptr, nullptr) };

        output.resize(outputLength);

        if (::WideCharToMultiByte(
                CP_UTF8, 0, input, inputLength, output.data(), outputLength, nullptr, nullptr)
            == 0) {
            throw std::runtime_error(glow::log::get_last_error<std::string>());
        }
    }

    return output;
}

auto wchar_to_string(const wchar_t* input, size_t length) -> std::string {
    std::string output;

    if (length > 0) {
        int inputLength { glow::math::check_safe_size<int>(length) };

        auto outputLength { ::WideCharToMultiByte(
            CP_UTF8, 0, input, inputLength, nullptr, 0, nullptr, nullptr) };

        output.resize(outputLength);

        if (::WideCharToMultiByte(
                CP_UTF8, 0, input, inputLength, output.data(), outputLength, nullptr, nullptr)
            == 0) {
            throw std::runtime_error(glow::log::get_last_error<std::string>());
        }
    }

    return output;
}

auto wchar_to_u8string(const wchar_t* input, size_t length) -> std::u8string {
    std::u8string output;

    if (length > 0) {
        int inputLength { glow::math::check_safe_size<int>(length) };

        auto outputLength { ::WideCharToMultiByte(
            CP_UTF8, 0, input, inputLength, nullptr, 0, nullptr, nullptr) };

        output.resize(outputLength);

        if (::WideCharToMultiByte(CP_UTF8,
                                  0,
                                  input,
                                  inputLength,
                                  reinterpret_cast<char*>(output.data()),
                                  outputLength,
                                  nullptr,
                                  nullptr)
            == 0) {
            throw std::runtime_error(glow::log::get_last_error<std::string>());
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
            throw std::runtime_error(glow::log::get_last_error<std::string>());
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
            throw std::runtime_error(glow::log::get_last_error<std::string>());
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
String::String(const char8_t* u8string)
    : storage { u8string } { }

String::String(const std::u8string& u8string)
    : storage { u8string } { }

String::String(std::u8string&& u8string)
    : storage { std::move(u8string) } { }

String::String(const std::filesystem::path& path)
    : storage { path.u8string() } { }

String::String(const std::u16string& u16string)
    : storage { to_u8string(u16string) } { }

String::String(const std::string& string)
    : storage { to_u8string(string) } { }

String::String(const std::wstring& wstring)
    : storage { to_u8string(wstring) } { }

auto String::operator()(const std::u8string& u8string) -> const std::u8string& {
    storage = u8string;

    return storage;
}

auto String::operator()(std::u8string&& u8string) -> const std::u8string& {
    storage = std::move(u8string);

    return storage;
}

auto String::operator()() const -> const std::u8string& { return storage; }

auto String::u8string() const -> std::u8string { return storage; }

auto String::u16string() const -> std::u16string { return to_u16string(storage); }

auto String::string() const -> std::string { return to_string(storage); }

auto String::wstring() const -> std::wstring { return to_wstring(storage); }
}; // namespace glow::text
