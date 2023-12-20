// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "WebViewWindow.hxx"
#include <gui/webview_window.hxx>

auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    glow::Window window;
    // window.create();
    // glow::Window window{"App"};
    // glow::gui::WebViewWindow wv;
    glow::gui::WebViewWindow wv{"Test2", window.m_hwnd.get(), 1};

    glow::gui::message_loop();

    return 0;
}
