// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "console.hxx"

//==============================================================================
namespace glow::console
{

//==============================================================================
Console::Console()
{
    ::AllocConsole();

    ::EnableMenuItem(::GetSystemMenu(::GetConsoleWindow(), FALSE), SC_CLOSE,
                     MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

    ::freopen_s(std::out_ptr(file), "CONOUT$", "w", stdout);
    ::freopen_s(std::out_ptr(file), "CONOUT$", "w", stderr);
    ::freopen_s(std::out_ptr(file), "CONIN$", "r", stdin);

    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
}

//==============================================================================
Console::~Console() { ::FreeConsole(); }

//==============================================================================
auto get_argv() -> std::vector<std::string>
{
    int argc{0};
    auto wideArgs{::CommandLineToArgvW(::GetCommandLineW(), &argc)};

    std::vector<std::string> argv;
    argv.reserve(sizeof(wideArgs));

    for (int i = 0; i < argc; i++)
    {
        auto arg{glow::text::narrow(wideArgs[i])};
        argv.push_back(arg);
    }

    ::LocalFree(wideArgs);

    return argv;
}

//==============================================================================
} // namespace glow::console
