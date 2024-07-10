// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <string>

namespace glow::text {
auto utf8_to_utf16(std::string_view utf8) -> std::wstring;
auto utf16_to_utf8(std::wstring_view utf16) -> std::string;
auto guid_to_wstring(const ::GUID& guid) -> std::wstring;
auto guid_to_string(const ::GUID& guid) -> std::string;
}; // namespace glow::text
