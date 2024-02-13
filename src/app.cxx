// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "app.hxx"

namespace glow
{
App::App(std::string name, intptr_t id) : m_id{id}
{
    ::WNDCLASSEXA wcex{sizeof(::WNDCLASSEXA)};

    if (!::GetClassInfoExA(::GetModuleHandleA(nullptr), name.c_str(), &wcex))
    {
        wcex.lpszClassName = name.c_str();
        wcex.lpszMenuName = 0;
        wcex.lpfnWndProc = WndProc;
        wcex.style = 0;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(intptr_t);
        wcex.hInstance = ::GetModuleHandleA(nullptr);
        wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        wcex.hCursor = static_cast<HCURSOR>(
            ::LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
        wcex.hIcon = static_cast<HICON>(
            ::LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
        wcex.hIconSm = static_cast<HICON>(
            ::LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));

        if (::RegisterClassExA(&wcex) == 0) throw std::runtime_error("Class registration failure");
    }

    if (::CreateWindowExA(0, wcex.lpszClassName, wcex.lpszClassName, 0, 0, 0, 0, 0, HWND_MESSAGE,
                          nullptr, ::GetModuleHandleA(nullptr), this) == nullptr)
        throw std::runtime_error("Window creation failure");
}

auto App::operator()() -> int
{
    ::MSG msg{};
    int r{};

    while ((r = ::GetMessageA(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) { return -1; }

        else
        {
            ::TranslateMessage(&msg);
            ::DispatchMessageA(&msg);
        }
    }

    return 0;
}

auto App::hwnd() const -> HWND { return m_hwnd.get(); }

auto App::id() const -> intptr_t { return m_id; }

auto App::close() -> int
{
    m_hwnd.reset();

    return 0;
}

auto App::quit() -> int
{
    ::PostQuitMessage(0);

    return 0;
}

auto CALLBACK App::WndProc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    auto self{instance_from_wnd_proc<App>(hWnd, uMsg, lParam)};

    if (self) { return self->default_wnd_proc(hWnd, uMsg, wParam, lParam); }

    else { return ::DefWindowProcA(hWnd, uMsg, wParam, lParam); }
}

auto App::default_wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    switch (uMsg)
    {
    case WM_CLOSE: return close();
    case WM_DESTROY: return quit();
    }

    return wnd_proc(hWnd, uMsg, wParam, lParam);
}

auto App::wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    return ::DefWindowProcA(hWnd, uMsg, wParam, lParam);
}
} // namespace glow
