// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <string>

namespace glow::text {
auto to_string(std::wstring_view input) -> std::string;
auto to_wstring(std::string_view input) -> std::wstring;
auto to_string(std::u8string_view input) -> std::string;
auto to_wstring(std::u8string_view input) -> std::wstring;
auto to_u8string(std::string_view input) -> std::u8string;
auto to_u8string(std::wstring_view input) -> std::u8string;
}; // namespace glow::text
