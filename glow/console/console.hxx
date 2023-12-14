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

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <wil/resource.h>

#include <text/text.hxx>

//==============================================================================
namespace glow::console
{

//==============================================================================
struct Console
{
    Console();
    ~Console();

  private:
    struct FILE_DELETER
    {
        auto operator()(FILE* pFile) noexcept -> void { fclose(pFile); }
    };
    using file_ptr = std::unique_ptr<FILE, FILE_DELETER>;
    file_ptr p_file;
};

//==============================================================================
auto get_argv() -> std::vector<std::string>;

//==============================================================================
} // namespace glow::console
