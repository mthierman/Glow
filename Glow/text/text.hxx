// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <limits>
#include <random>
#include <string>

namespace glow::text
{

auto narrow(std::wstring utf16) -> std::string;
auto widen(std::string utf8) -> std::wstring;
auto randomize(std::string string) -> std::string;

} // namespace glow::text
