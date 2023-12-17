// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "logging.hxx"

namespace glow::logging
{

auto debug(std::string string, std::source_location location) -> void
{
    OutputDebugStringA(("file_name: " + std::string(location.file_name()) + "\n").c_str());
    OutputDebugStringA(("function_name: " + std::string(location.function_name()) + "\n").c_str());
    OutputDebugStringA(("line: " + std::to_string(location.line()) + "\n").c_str());
    OutputDebugStringA(("column: " + std::to_string(location.column()) + "\n").c_str());
    OutputDebugStringA((string + "\n").c_str());
}

auto msgbox(std::string string) -> void { MessageBoxA(nullptr, string.c_str(), "Message", 0); }

auto errorbox(std::string string) -> void
{
    std::string error = string + ". Error: " + std::to_string(GetLastError());
    MessageBoxA(nullptr, error.c_str(), "Error", 0);
}

auto hr(HRESULT hresult) -> void
{
    std::string message{std::system_category().message(hresult)};
    std::println("{}", message);
}

} // namespace glow::logging
