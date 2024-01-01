// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <comdef.h>
#include <shellapi.h>

#include <filesystem>
#include <print>
#include <source_location>
#include <string>

#include <filesystem/filesystem.hxx>
#include <text/text.hxx>

namespace glow::log
{

auto debug(std::string message, std::source_location location = std::source_location::current())
    -> void;
auto print(std::string message, std::source_location location = std::source_location::current())
    -> void;

auto box(std::string message, UINT type = MB_OK | MB_ICONINFORMATION) -> int;
auto shell(std::string message, UINT type = MB_OK | MB_ICONINFORMATION) -> int;
auto stock(std::string message, SHSTOCKICONID icon = SIID_INFO) -> void;

} // namespace glow::log
