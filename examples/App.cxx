// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "App.hxx"
#include "Window.hxx"

#include <gui/webview.hxx>

auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    App app;
    App app2;
    glow::gui::WebView2 wv1{app.m_hwnd.get(), 1};
    glow::gui::WebView2 wv2{app2.m_hwnd.get(), 2};

    Window window;
    glow::gui::WebView2 wv3{window.m_hwnd.get(), 1};
    glow::gui::WebView2 wv4{window.m_hwnd.get(), 2};

    Window window2;
    glow::gui::WebView2 wv5{window2.m_hwnd.get(), 1};
    glow::gui::WebView2 wv6{window2.m_hwnd.get(), 2};

    glow::gui::message_loop();

    return 0;
}
