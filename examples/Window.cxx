// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <Windows.h>

#include <bit>

#include <gui/gui.hxx>

auto size() -> int
{
    OutputDebugStringA("RESIZE\n");
    return 0;
}

auto close() -> int
{
    PostQuitMessage(0);
    return 0;
}

auto CALLBACK new_wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    // case WM_SIZE: OutputDebugStringA("RESIZE\n"); return 0;
    // case WM_CLOSE: PostQuitMessage(0); return 0;
    case WM_SIZE: size();
    case WM_CLOSE: close();
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    auto atom{glow::gui::register_window()};
    wil::unique_hwnd hwnd{glow::gui::create_window(atom)};
    glow::gui::show_window(hwnd.get());
    SetWindowLongPtrA(hwnd.get(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(new_wnd_proc));

    glow::gui::message_loop();

    return 0;
}
