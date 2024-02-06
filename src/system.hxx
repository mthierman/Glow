// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <shellapi.h>
#include <comdef.h>

#include <filesystem>
#include <iostream>
#include <print>
#include <source_location>
#include <system_error>
#include <vector>

#include <wil/resource.h>

#include "text.hxx"
#include "filesystem.hxx"

namespace glow
{
auto argv(int argc, char* argv[]) -> std::vector<std::string>;
auto cmd_to_argv() -> std::vector<std::string>;
auto format_message(::HRESULT errorCode) -> std::string;
auto system_error_string(::HRESULT errorCode) -> std::string;
auto hresult_string(::HRESULT errorCode) -> std::string;
auto last_error() -> ::HRESULT;
auto last_error_string() -> std::string;
auto hresult_check(::HRESULT errorCode) -> ::HRESULT;
auto hresult_debug(::HRESULT errorCode,
                   std::source_location location = std::source_location::current()) -> void;
auto hresult_print(::HRESULT errorCode,
                   std::source_location location = std::source_location::current()) -> void;
auto source_debug(std::string message,
                  std::source_location location = std::source_location::current()) -> void;
auto source_print(std::string message,
                  std::source_location location = std::source_location::current()) -> void;
auto message_box(std::string message, unsigned int type = MB_OK | MB_ICONINFORMATION) -> int;
auto message_box_shell(std::string message, unsigned int type = MB_OK | MB_ICONINFORMATION) -> int;
auto message_box_stock(std::string message, ::SHSTOCKICONID icon = ::SIID_INFO) -> void;
auto create_process(std::filesystem::path path) -> int;
} // namespace glow
