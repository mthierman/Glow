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

#include <wil/resource.h>

#include <iostream>
#include <string>
#include <vector>

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

} // namespace glow::console
