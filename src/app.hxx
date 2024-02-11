// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <stdexcept>
#include <string>

#include <wil/resource.h>

#include "random.hxx"

namespace glow
{
template <typename T> struct App
{
    App(std::string name = "App", intptr_t id = glow::random<intptr_t>()) : m_id{id}
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
            wcex.hIcon = static_cast<HICON>(::LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                                         LR_SHARED | LR_DEFAULTSIZE));
            wcex.hIconSm = static_cast<HICON>(::LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0,
                                                           0, LR_SHARED | LR_DEFAULTSIZE));

            if (::RegisterClassExA(&wcex) == 0)
                throw std::runtime_error("Class registration failure");
        }

        if (::CreateWindowExA(0, wcex.lpszClassName, wcex.lpszClassName, 0, 0, 0, 0, 0,
                              HWND_MESSAGE, nullptr, ::GetModuleHandleA(nullptr), this) == nullptr)
            throw std::runtime_error("Window creation failure");
    }

    virtual ~App() {}

    auto operator()() -> int
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

    auto hwnd() const -> HWND { return m_hwnd.get(); }

    auto id() const -> intptr_t { return m_id; }

    auto close() -> int
    {
        m_hwnd.reset();

        return 0;
    }

    auto quit() -> int
    {
        ::PostQuitMessage(0);

        return 0;
    }

  private:
    static auto CALLBACK WndProc(::HWND hwnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT
    {
        T* self{nullptr};

        if (message == WM_NCCREATE)
        {
            auto lpCreateStruct{reinterpret_cast<::CREATESTRUCTA*>(lParam)};
            self = static_cast<T*>(lpCreateStruct->lpCreateParams);
            self->m_hwnd.reset(hwnd);
            ::SetWindowLongPtrA(hwnd, 0, reinterpret_cast<intptr_t>(self));
        }

        else { self = reinterpret_cast<T*>(::GetWindowLongPtrA(hwnd, 0)); }

        if (self) { return self->default_wnd_proc(hwnd, message, wParam, lParam); }

        else { return ::DefWindowProcA(hwnd, message, wParam, lParam); }
    }

    virtual auto default_wnd_proc(::HWND hwnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT
    {
        switch (message)
        {
        case WM_CLOSE: return close();
        case WM_DESTROY: return quit();
        }

        return wnd_proc(hwnd, message, wParam, lParam);
    }

    virtual auto wnd_proc(::HWND hwnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT
    {
        return ::DefWindowProcA(hwnd, message, wParam, lParam);
    }

  public:
    intptr_t m_id;
    wil::unique_hwnd m_hwnd;
};
} // namespace glow
