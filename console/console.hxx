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

#ifdef _DEBUG
    ::AllocConsole();
    ::SetConsoleTitleW(L"Console");
    ::EnableMenuItem(::GetSystemMenu(::GetConsoleWindow(), FALSE), SC_CLOSE,
                     MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    // ::GetWindowLongPtrW(::GetConsoleWindow(), GWL_STYLE);
    // ::SetWindowLongPtrW(::GetConsoleWindow(), GWL_STYLE,
    //                     ::GetWindowLongPtrW(::GetConsoleWindow(), GWL_STYLE) & ~WS_CAPTION &
    //                         ~WS_SIZEBOX & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX);
    // ::SetWindowLongPtrW(::GetConsoleWindow(), GWL_EXSTYLE, WS_EX_TOOLWINDOW);
    // ::SetWindowPos(::GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    ::freopen_s(&f, "CONOUT$", "w", stdout);
    ::freopen_s(&f, "CONOUT$", "w", stderr);
    ::freopen_s(&f, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
#endif

    return f;
}

auto remove_console(FILE* f) -> void
{
#ifdef _DEBUG
    ::fclose(f);
    ::FreeConsole();
#endif
}
} // namespace glow::console
