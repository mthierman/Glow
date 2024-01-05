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

#include <wil/resource.h>

#include <iostream>
#include <print>
#include <source_location>
#include <string>
#include <vector>

#include <glow/filesystem.hxx>
#include <glow/text.hxx>

namespace glow::console
{

struct Console
{
    Console();
    ~Console();

    FILE* pFile;
};

auto argv() -> std::vector<std::string>;

auto hresult_to_string(HRESULT errorCode) -> std::string;
auto hresult_check(HRESULT errorCode) -> HRESULT;
auto debug_hresult(HRESULT errorCode,
                   std::source_location location = std::source_location::current()) -> void;
auto print_hresult(HRESULT errorCode,
                   std::source_location location = std::source_location::current()) -> void;

auto debug(std::string message, std::source_location location = std::source_location::current())
    -> void;
auto print(std::string message, std::source_location location = std::source_location::current())
    -> void;

auto box(std::string message, UINT type = MB_OK | MB_ICONINFORMATION) -> int;
auto shell(std::string message, UINT type = MB_OK | MB_ICONINFORMATION) -> int;
auto stock(std::string message, SHSTOCKICONID icon = SIID_INFO) -> void;

} // namespace glow::console
