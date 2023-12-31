// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <console/console.hxx>

namespace glow::console
{

Console::Console()
{
    AllocConsole();

    EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE,
                   MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

    wil::unique_file uFile;
    auto pFile = uFile.get();

    freopen_s(std::out_ptr(pFile), "CONIN$", "r", stdin);
    freopen_s(std::out_ptr(pFile), "CONOUT$", "w", stdout);
    freopen_s(std::out_ptr(pFile), "CONOUT$", "w", stderr);

    std::cin.clear();
    std::cout.clear();
    std::cerr.clear();
    std::clog.clear();
}

Console::~Console() { FreeConsole(); }

// Console::Console()
// {
//     AllocConsole();

//     EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE,
//                    MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

//     auto file = p_file.get();

//     freopen_s(std::out_ptr(file), "CONOUT$", "w", stdout);
//     freopen_s(std::out_ptr(file), "CONOUT$", "w", stderr);
//     freopen_s(std::out_ptr(file), "CONIN$", "r", stdin);

//     std::cout.clear();
//     std::clog.clear();
//     std::cerr.clear();
//     std::cin.clear();
// }

// Console::~Console() { FreeConsole(); }

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
