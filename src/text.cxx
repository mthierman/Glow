// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/text.hxx>

#include <Windows.h>

#include <icu.h>

#include <glow/log.hxx>

namespace glow::text {
auto u16string(std::u8string_view input) -> std::optional<std::u16string> {
    std::u16string buffer;
    buffer.resize(input.length());
    auto errorCode { U_ZERO_ERROR };

    u_strFromUTF8(buffer.data(),
                  static_cast<int32_t>(buffer.length()),
                  nullptr,
                  reinterpret_cast<const char*>(input.data()),
                  static_cast<int32_t>(input.length()),
                  &errorCode);

    if (U_SUCCESS(errorCode)) {
        return buffer;
    }

    return std::nullopt;
}

auto u8string(std::u16string_view input) -> std::optional<std::u8string> {
    std::u8string buffer;
    buffer.resize(input.length());
    auto errorCode { U_ZERO_ERROR };

    u_strToUTF8(reinterpret_cast<char*>(buffer.data()),
                static_cast<int32_t>(buffer.length()),
                nullptr,
                input.data(),
                static_cast<int32_t>(input.length()),
                &errorCode);

    if (U_SUCCESS(errorCode)) {
        return buffer;
    }

    return std::nullopt;
}

auto u16string(std::string_view input) -> std::optional<std::u16string> {
    if (auto converted { u16string(reinterpret_cast<const char8_t*>(input.data())) }) {
        return *converted;
    }

    return std::nullopt;
}

auto u8string(std::wstring_view input) -> std::optional<std::u8string> {
    if (auto converted { u8string(reinterpret_cast<const char16_t*>(input.data())) }) {
        return *converted;
    }

    return std::nullopt;
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
    if (auto converted { u16string(input) }; converted) {
        std::u16string buffer;
        buffer.resize(input.length());
        auto errorCode { U_ZERO_ERROR };

        u_strToUpper(buffer.data(),
                     static_cast<int32_t>(buffer.length()),
                     converted.value().c_str(),
                     static_cast<int32_t>(converted.value().length()),
                     nullptr,
                     &errorCode);

        if (auto output { u8string(buffer) }) {
            return *output;
        }
    }

    return std::nullopt;
}

auto lower(std::u8string_view input) -> std::optional<std::u8string> {
    if (auto converted { u16string(input) }; converted) {
        std::u16string buffer;
        buffer.resize(input.length());
        auto errorCode { U_ZERO_ERROR };

        u_strToLower(buffer.data(),
                     static_cast<int32_t>(buffer.length()),
                     converted.value().c_str(),
                     static_cast<int32_t>(converted.value().length()),
                     nullptr,
                     &errorCode);

        if (auto output { u8string(buffer) }) {
            return *output;
        }
    }

    return std::nullopt;
}
}; // namespace glow::text
