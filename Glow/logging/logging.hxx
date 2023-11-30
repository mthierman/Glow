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
#include <source_location>
#include <string>
#include <system_error>

#include <text/text.hxx>

namespace glow::logging
{
void debug(std::string in, std::source_location location = std::source_location::current())
{
    ::OutputDebugString(("file_name: " + std::string(location.file_name()) + "\n").c_str());
    ::OutputDebugString(("function_name: " + std::string(location.function_name()) + "\n").c_str());
    ::OutputDebugString(("line: " + std::to_string(location.line()) + "\n").c_str());
    ::OutputDebugString(("column: " + std::to_string(location.column()) + "\n").c_str());
    ::OutputDebugString((in + "\n").c_str());
}

void msgbox(std::string string) { ::MessageBox(nullptr, string.c_str(), "Message", 0); };

int errorbox(std::string string)
{
    std::string error = string + ". Error: " + std::to_string(::GetLastError());
    ::MessageBox(nullptr, error.c_str(), "Error", 0);

    return 0;
};

auto hr(HRESULT hResult) -> void
{
    std::string errorMsg{std::system_category().message(hResult)};
    std::println("{}", errorMsg);
}
} // namespace glow::logging
