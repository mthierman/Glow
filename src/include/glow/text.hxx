// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <format>
#include <string>

namespace glow::text {
auto to_string(std::u8string_view input) -> std::string;
auto to_wstring(std::u16string_view input) -> std::wstring;

auto to_string(const wchar_t* input, size_t length) -> std::string;
auto to_wstring(const char* input, size_t length) -> std::wstring;

auto to_string(std::u16string_view input) -> std::string;
auto to_string(std::wstring_view input) -> std::string;

auto to_wstring(std::u8string_view input) -> std::wstring;
auto to_wstring(std::string_view input) -> std::wstring;

auto to_u8string(std::string_view input) -> std::u8string;
auto to_u8string(std::wstring_view input) -> std::u8string;

auto to_u16string(std::string_view input) -> std::u16string;
auto to_u16string(std::wstring_view input) -> std::u16string;
}; // namespace glow::text

namespace std {
template <> struct formatter<std::u8string> : formatter<string_view> {
    auto format(const std::u8string& u8string, format_context& context) const noexcept {
        return formatter<string_view>::format(glow::text::to_string(u8string), context);
    }
};

template <> struct formatter<std::u8string, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::u8string& u8string, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(glow::text::to_wstring(u8string), context);
    }
};

template <> struct formatter<std::u16string> : formatter<string_view> {
    auto format(const std::u16string& u16string, format_context& context) const noexcept {
        return formatter<string_view>::format(glow::text::to_string(u16string), context);
    }
};

template <> struct formatter<std::u16string, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::u16string& u16string, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(glow::text::to_wstring(u16string), context);
    }
};
} // namespace std
