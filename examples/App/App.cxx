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

using glow::gui::WebView2;

auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    App app;
    glow::gui::set_title(app.m_hwnd.get(), "App");

    std::vector<std::unique_ptr<glow::gui::WebView2>> app_webviews;
    app_webviews.push_back(std::make_unique<WebView2>(app.m_hwnd.get(), 1));
    app_webviews.push_back(std::make_unique<WebView2>(app.m_hwnd.get(), 2));

    glow::gui::set_border(app_webviews.at(0)->m_hwnd.get(), true);
    glow::gui::set_border(app_webviews.at(1)->m_hwnd.get(), true);

    Window window;
    glow::gui::set_title(window.m_hwnd.get(), "Window");

    std::vector<std::unique_ptr<glow::gui::WebView2>> window_webviews;
    window_webviews.push_back(std::make_unique<WebView2>(window.m_hwnd.get(), 1));
    window_webviews.push_back(std::make_unique<WebView2>(window.m_hwnd.get(), 2));

    glow::gui::set_border(window_webviews.at(0)->m_hwnd.get(), true);
    glow::gui::set_border(window_webviews.at(1)->m_hwnd.get(), true);

    glow::gui::message_loop();

    return 0;
}
