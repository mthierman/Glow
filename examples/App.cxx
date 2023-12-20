// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <bit>
#include <gui/window.hxx>
#include <gui/webview.hxx>

struct TestApp final : public glow::gui::Window
{
    using glow::gui::Window::Window;

    auto handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_destroy() -> int;
    auto on_notify() -> int;
    auto on_size() -> int;

    static auto enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL;
};

auto TestApp::handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_DESTROY: return on_destroy();
    case WM_NOTIFY: return on_notify();
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto TestApp::on_destroy() -> int
{
    PostQuitMessage(0);

    return 0;
}

auto TestApp::on_notify() -> int
{
    on_size();

    return 0;
}

auto TestApp::on_size() -> int
{
    RECT clientRect{0};
    GetClientRect(m_hwnd.get(), &clientRect);
    EnumChildWindows(m_hwnd.get(), enum_child_proc, std::bit_cast<LPARAM>(&clientRect));
    // Sleep(1);

    return 0;
}

auto CALLBACK TestApp::enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL
{
    auto childId{GetWindowLongPtrA(hwnd, GWL_ID)};

    auto rcParent{(LPRECT)lParam};

    if (childId == 1)
    {
        SetWindowPos(hwnd, nullptr, 0, 0, (rcParent->right - rcParent->left),
                     (rcParent->bottom - rcParent->top), SWP_NOZORDER);
        Sleep(1);
    }

    return 1;
}

auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    TestApp app;
    glow::gui::WebView wv{"WebView2", app.m_hwnd.get(), 1};
    // glow::Window window;
    // glow::Window window2{"MainWindow2"};

    // glow::gui::WebViewWindow wv;
    // glow::gui::WebViewWindow wv{"Test2", window.m_hwnd.get(), 1};

    glow::gui::message_loop();

    return 0;
}
