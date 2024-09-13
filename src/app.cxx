// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/app.hxx>

#include <Windows.h>

#include <stdlib.h>

#include <string>

#include <glow/system.hxx>

namespace glow::app {
auto App::create() -> void {
    auto instance { glow::system::instance() };

    ::WNDCLASSEXW windowClass { .cbSize { sizeof(::WNDCLASSEXW) },
                                .style { 0 },
                                .lpfnWndProc { procedure },
                                .cbClsExtra { 0 },
                                .cbWndExtra { sizeof(App) },
                                .hInstance { instance },
                                .hIcon { nullptr },
                                .hCursor { nullptr },
                                .hbrBackground { nullptr },
                                .lpszMenuName { nullptr },
                                .lpszClassName { L"App" },
                                .hIconSm { nullptr } };

    if (::GetClassInfoExW(instance, windowClass.lpszClassName, &windowClass) == 0) {
        ::RegisterClassExW(&windowClass);
    }

    ::CreateWindowExW(0,
                      windowClass.lpszClassName,
                      L"",
                      0,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      HWND_MESSAGE,
                      nullptr,
                      instance,
                      this);
}

auto CALLBACK App::procedure(::HWND hwnd,
                             ::UINT msg,
                             ::WPARAM wparam,
                             ::LPARAM lparam) -> ::LRESULT {
    if (msg == WM_NCCREATE) {
        auto create { reinterpret_cast<::CREATESTRUCTW*>(lparam) };

        if (auto self { static_cast<App*>(create->lpCreateParams) }; self) {
            ::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<::LONG_PTR>(self));
            self->hwnd.reset(hwnd);
        }
    }

    if (auto self { reinterpret_cast<App*>(::GetWindowLongPtrW(hwnd, 0)) }; self) {
        if (msg == WM_NCDESTROY) {
            ::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<::LONG_PTR>(nullptr));
        }

        if (self->messages.contains(msg)) {
            return self->messages.invoke({ hwnd, msg, wparam, lparam });
        }
    }

    return glow::message::default_procedure({ hwnd, msg, wparam, lparam });
}

auto App::notify_app(glow::message::Code code,
                     std::string_view message,
                     ::HWND receiverHwnd) -> void {
    messages.notify(code, message, hwnd.get(), id, receiverHwnd);
}

auto run() -> int {
    ::MSG msg {};
    int r {};

    while ((r = ::GetMessageW(&msg, nullptr, 0, 0)) != 0) {
        if (r == -1) {
            return EXIT_FAILURE;
        }

        else {
            ::TranslateMessage(&msg);
            ::DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}
}; // namespace glow::app
