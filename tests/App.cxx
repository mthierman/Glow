// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman mthierman@gmail.com
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝

#include <Windows.h>

#include <console/console.hxx>
#include <gui/app.hxx>

auto WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/,
                     int /*nCmdShow*/) -> int
{
    // #ifdef _DEBUG
    //     glow::console::Console console;
    // #endif

    auto app{std::make_unique<glow::gui::App>("App")};

    MSG msg;
    int r;

    while ((r = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) return 0;

        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}
