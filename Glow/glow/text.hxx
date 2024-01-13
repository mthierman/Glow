// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <objbase.h>

#include <random>
#include <string>

#include <wil/win32_helpers.h>

namespace glow
{
namespace text
{
auto narrow(std::wstring utf16) -> std::string;
auto widen(std::string utf8) -> std::wstring;
auto randomize(std::string string) -> std::string;
auto random_int64() -> int64_t;
auto random_int32() -> int32_t;
auto create_guid() -> GUID;
auto guid_string(GUID guid) -> std::string;
} // namespace text
} // namespace glow
