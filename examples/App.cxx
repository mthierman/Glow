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

struct App final : public glow::gui::Window
{
    auto handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_destroy() -> int;
    auto on_notify() -> int;
    auto on_size() -> int;

    static auto enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL;
};

auto App::handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_DESTROY: return on_destroy();
    case WM_NOTIFY: return on_notify();
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto App::on_destroy() -> int
{
    PostQuitMessage(0);

    return 0;
}

auto App::on_notify() -> int
{
    on_size();

    return 0;
}

auto App::on_size() -> int
{
    RECT clientRect{0};
    GetClientRect(m_hwnd.get(), &clientRect);
    EnumChildWindows(m_hwnd.get(), enum_child_proc, std::bit_cast<LPARAM>(&clientRect));
    // Sleep(1);

    return 0;
}

auto CALLBACK App::enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL
{
    auto childId{GetWindowLongPtrA(hwnd, GWL_ID)};

    auto rcParent{(LPRECT)lParam};

    if (childId == 1)
    {
        SetWindowPos(hwnd, nullptr, 0, 0, (rcParent->right - rcParent->left),
                     (rcParent->bottom - rcParent->top), SWP_NOZORDER);
        // Sleep(1);
    }

    return 1;
}

auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    App app;
    app.create();
    glow::gui::WebView2 wv{app.m_hwnd.get(), 1};
    wv.create();

    glow::gui::message_loop();

    return 0;
}
