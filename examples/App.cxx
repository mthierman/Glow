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

    std::vector<std::unique_ptr<glow::gui::WebView2>> webviews;
    webviews.push_back(std::make_unique<glow::gui::WebView2>(app.m_hwnd.get(), 1));
    webviews.push_back(std::make_unique<glow::gui::WebView2>(app.m_hwnd.get(), 2));

    glow::gui::set_border(webviews.at(0)->m_hwnd.get(), true);
    glow::gui::set_border(webviews.at(1)->m_hwnd.get(), true);

    glow::gui::message_loop();

    return 0;
}
