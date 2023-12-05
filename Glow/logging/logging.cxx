// clang-format off
// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝
// clang-format on

#include <logging/logging.hxx>

//==============================================================================
namespace glow::logging
{
auto debug(std::string string, std::source_location location) -> void
{
    ::OutputDebugString(("file_name: " + std::string(location.file_name()) + "\n").c_str());
    ::OutputDebugString(("function_name: " + std::string(location.function_name()) + "\n").c_str());
    ::OutputDebugString(("line: " + std::to_string(location.line()) + "\n").c_str());
    ::OutputDebugString(("column: " + std::to_string(location.column()) + "\n").c_str());
    ::OutputDebugString((string + "\n").c_str());
}

auto msgbox(std::string string) -> void { ::MessageBox(nullptr, string.c_str(), "Message", 0); }

auto errorbox(std::string string) -> void
{
    std::string error = string + ". Error: " + std::to_string(::GetLastError());
    ::MessageBox(nullptr, error.c_str(), "Error", 0);
}

auto hr(HRESULT hresult) -> void
{
    std::string errorMsg{std::system_category().message(hresult)};
    std::println("{}", errorMsg);
}
} // namespace glow::logging
