// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <string>

namespace glow::guid {
auto create() -> ::GUID;
auto to_wstring(const ::GUID& guid) -> std::wstring;
auto to_string(const ::GUID& guid) -> std::string;
}; // namespace glow::guid
