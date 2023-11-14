#pragma once

#include <Windows.h>

#include <iostream>
#include <string>
#include <vector>

#include "../text/text.hxx"

namespace glow::console
{
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

auto create_console() -> FILE*
{
    FILE* f{nullptr};
    ::AllocConsole();
    ::EnableMenuItem(::GetSystemMenu(::GetConsoleWindow(), FALSE), SC_CLOSE,
                     MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    ::freopen_s(&f, "CONOUT$", "w", stdout);
    ::freopen_s(&f, "CONOUT$", "w", stderr);
    ::freopen_s(&f, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    return f;
}

auto remove_console(FILE* f) -> void
{
    ::fclose(f);
    ::FreeConsole();
}
} // namespace glow::console
