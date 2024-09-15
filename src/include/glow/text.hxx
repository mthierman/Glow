// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <filesystem>
#include <format>
#include <string>

namespace glow::text {
auto wchar_to_string(const wchar_t* input, size_t length) -> std::string;
auto wchar_to_u8string(const wchar_t* input, size_t length) -> std::u8string;
auto char_to_wstring(const char* input, size_t length) -> std::wstring;
auto char_to_u16string(const char* input, size_t length) -> std::u16string;

auto to_string(std::string_view input) -> std::string;
auto to_wstring(std::wstring_view input) -> std::wstring;

auto to_string(std::wstring_view input) -> std::string;
auto to_wstring(std::string_view input) -> std::wstring;

auto to_string(std::u8string_view input) -> std::string;
auto to_wstring(std::u16string_view input) -> std::wstring;

auto to_string(std::u16string_view input) -> std::string;
auto to_wstring(std::u8string_view input) -> std::wstring;

auto to_u8string(std::u8string_view input) -> std::u8string;
auto to_u8string(std::string_view input) -> std::u8string;
auto to_u16string(std::u16string_view input) -> std::u16string;
auto to_u16string(std::wstring_view input) -> std::u16string;

auto to_u8string(std::wstring_view input) -> std::u8string;
auto to_u8string(std::u16string_view input) -> std::u8string;
auto to_u16string(std::string_view input) -> std::u16string;
auto to_u16string(std::u8string_view input) -> std::u16string;

struct String {
    String() = default;
    ~String() = default;
    String(const String& u8string) = default;
    String(String&& u8string) = default;
    explicit String(const std::u8string& u8string);
    explicit String(std::u8string&& u8string);
    explicit String(const std::filesystem::path& path);
    explicit String(std::filesystem::path&& path);

    auto operator()(std::u8string string) -> const std::u8string&;
    auto operator()() const -> const std::u8string&;

    auto u8string() const -> std::u8string;
    auto u16string() const -> std::u16string;
    auto string() const -> std::string;
    auto wstring() const -> std::wstring;

protected:
    std::u8string storage;
};
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
