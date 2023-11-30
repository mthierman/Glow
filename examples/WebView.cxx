// clang-format off
// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝
// clang-format on

#include <Windows.h>

#include <console/console.hxx>
#include <gui/app.hxx>
#include <gui/webview.hxx>

#include "WebView.hxx"

//==============================================================================
auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    // #ifdef _DEBUG
    //     glow::console::Console console;
    // #endif

    auto app{std::make_unique<glow::App>("WebView")};

    auto wv{std::make_unique<glow::gui::WebView>("WebView", app->get_hwnd(), 1)};

    MSG msg{nullptr};
    int r{0};

    while ((r = ::GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) return 0;

        else
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    return 0;
}
