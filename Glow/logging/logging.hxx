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

#include <print>
#include <source_location>
#include <string>
#include <system_error>

#include <text/text.hxx>

namespace glow::logging
{

auto com_error(HRESULT hResult) -> void;
auto debug(std::string string, std::source_location location = std::source_location::current())
    -> void;
auto box_msg(std::string message) -> void;
auto box_err(std::string message) -> void;
auto shell_msg(std::string message) -> void;
auto shell_err(std::string message) -> void;
auto hr(HRESULT hresult) -> void;

} // namespace glow::logging
