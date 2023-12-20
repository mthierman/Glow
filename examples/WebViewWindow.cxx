// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "WebViewWindow.hxx"
// #include <gui/webview_window.hxx>

struct TestApp final : public glow::gui::Window
{
    using glow::gui::Window::Window;
    auto handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_destroy() -> int;
};

auto TestApp::handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_DESTROY: return on_destroy();
    }

    return DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto TestApp::on_destroy() -> int
{
    PostQuitMessage(0);

    return 0;
}

auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    TestApp app;
    // glow::Window window;
    // glow::Window window2{"MainWindow2"};

    // glow::gui::WebViewWindow wv;
    // glow::gui::WebViewWindow wv{"Test2", window.m_hwnd.get(), 1};

    glow::gui::message_loop();

    return 0;
}
