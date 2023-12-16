// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "console.hxx"

namespace glow::console
{

Console::Console()
{
    AllocConsole();

    EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE,
                   MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

    wil::unique_file file;
    auto pFile = file.get();

    freopen_s(std::out_ptr(pFile), "CONIN$", "r", stdin);
    freopen_s(std::out_ptr(pFile), "CONOUT$", "w", stdout);
    freopen_s(std::out_ptr(pFile), "CONOUT$", "w", stderr);

    std::cin.clear();
    std::cout.clear();
    std::cerr.clear();
    std::clog.clear();
}

Console::~Console() { FreeConsole(); }

auto get_argv() -> std::vector<std::string>
{
    int argc{};
    wil::unique_hlocal_ptr<PWSTR[]> args;
    args.reset(CommandLineToArgvW(GetCommandLineW(), &argc));

    std::vector<std::string> argv;

    for (int i = 0; i < argc; i++)
    {
        auto arg{glow::text::narrow(args[i])};
        argv.push_back(arg);
    }

    return argv;
}

} // namespace glow::console
