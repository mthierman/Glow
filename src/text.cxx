// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/text.hxx>

#include <Windows.h>

#include <icu.h>

namespace glow::text {
auto u16string(std::u8string_view input) -> std::optional<std::u16string> {
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
        }
    }

    return std::nullopt;
}

auto u8string(std::u16string_view input) -> std::optional<std::u8string> {
    auto buffer { std::wstring(input.begin(), input.end()) };

    if (auto length { buffer.length() }; length > 0) {
        if (length > std::numeric_limits<int>::max()) {
            return std::nullopt;
        }

        auto outputLength { ::WideCharToMultiByte(
            CP_UTF8, 0, buffer.c_str(), static_cast<int>(length), nullptr, 0, nullptr, nullptr) };

        std::u8string output;
        output.resize(outputLength);

        if (::WideCharToMultiByte(CP_UTF8,
                                  0,
                                  buffer.c_str(),
                                  static_cast<int>(length),
                                  reinterpret_cast<char*>(output.data()),
                                  outputLength,
                                  nullptr,
                                  nullptr)
            != 0) {
            return output;
        }
    }

    return std::nullopt;
}

auto u16string(std::string_view input) -> std::optional<std::u16string> {
    auto converted { u16string(reinterpret_cast<const char8_t*>(input.data())) };

    if (!converted.has_value()) {
        return std::nullopt;
    }

    return converted.value();
}

auto u8string(std::wstring_view input) -> std::optional<std::u8string> {
    auto converted { u8string(reinterpret_cast<const char16_t*>(input.data())) };

    if (!converted.has_value()) {
        return std::nullopt;
    }

    return converted.value();
}

auto c_str(std::u8string& input) -> char* { return reinterpret_cast<char*>(input.data()); }

auto c_str(const std::u8string& input) -> const char* {
    return reinterpret_cast<const char*>(input.data());
}

auto c_str(std::u16string& input) -> wchar_t* { return reinterpret_cast<wchar_t*>(input.data()); }

auto c_str(const std::u16string& input) -> const wchar_t* {
    return reinterpret_cast<const wchar_t*>(input.data());
}

auto upper(std::u8string_view input) -> std::optional<std::u8string> {
    if (auto converted { u16string(input) }; converted.has_value()) {
        auto errorCode { U_ZERO_ERROR };
        auto length { u_strToUpper(nullptr, 0, converted.value().c_str(), -1, 0, &errorCode) };

        std::u16string buffer;
        buffer.resize(length + 1);
        errorCode = U_ZERO_ERROR;

        u_strToUpper(buffer.data(), length + 1, converted.value().c_str(), -1, 0, &errorCode);

        if (auto output { u8string(buffer) }; output.has_value()) {
            return output.value();
        }
    }

    return std::nullopt;
}

// auto lower(std::u8string_view input) -> std::u8string { }
}; // namespace glow::text
