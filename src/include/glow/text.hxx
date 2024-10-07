// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

// #include <filesystem>
// #include <format>
#include <optional>
#include <string>

namespace glow::text {
auto u16string(std::u8string_view input) -> std::optional<std::u16string>;
auto u8string(std::u16string_view input) -> std::optional<std::u8string>;
auto u16string(std::string_view input) -> std::optional<std::u16string>;
auto u8string(std::wstring_view input) -> std::optional<std::u8string>;

auto c_str(std::u8string_view input) -> const char*;
auto c_str(std::u16string_view input) -> const wchar_t*;
}; // namespace glow::text

// namespace std {
// template <> struct formatter<std::u8string> : formatter<string_view> {
//     auto format(const std::u8string& u8string, format_context& context) const noexcept {
//         return formatter<string_view>::format(glow::text::to_string(u8string), context);
//     }
// };

// template <> struct formatter<std::u8string, wchar_t> : formatter<wstring_view, wchar_t> {
//     auto format(const std::u8string& u8string, wformat_context& context) const noexcept {
//         return formatter<wstring_view, wchar_t>::format(glow::text::to_wstring(u8string),
//         context);
//     }
// };

// template <> struct formatter<std::u16string> : formatter<string_view> {
//     auto format(const std::u16string& u16string, format_context& context) const noexcept {
//         return formatter<string_view>::format(glow::text::to_string(u16string), context);
//     }
// };

// template <> struct formatter<std::u16string, wchar_t> : formatter<wstring_view, wchar_t> {
//     auto format(const std::u16string& u16string, wformat_context& context) const noexcept {
//         return formatter<wstring_view, wchar_t>::format(glow::text::to_wstring(u16string),
//         context);
//     }
// };
// } // namespace std
