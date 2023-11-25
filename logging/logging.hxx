// clang-format off
// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <print>
#include <string>
#include <system_error>

#include <text/text.hxx>

namespace glow::logging
{
void debug(std::string in) { ::OutputDebugString(in.c_str()); }

void debugln(std::string in) { ::OutputDebugString((in + "\n").c_str()); }

void msgbox(std::string in) { ::MessageBox(nullptr, in.c_str(), "Message", 0); };

int errorbox(std::string in)
{
    std::string error = in + ". Error: " + std::to_string(GetLastError());
    ::MessageBox(nullptr, error.c_str(), "Error", 0);

    return 0;
};

auto hr(HRESULT hresult) -> void
{
    std::string errormsg{std::system_category().message(hresult)};
    std::println("{}", errormsg);
}
} // namespace glow::logging
