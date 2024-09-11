// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/log.hxx>
#include <glow/math.hxx>
#include <glow/system.hxx>
#include <glow/window.hxx>

#include <stdexcept>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.UI.ViewManagement.h>

namespace glow {
auto Window::create(const std::string& title) -> void {
    auto className { std::wstring(L"Window") };
    auto instance { glow::system::instance() };
    auto resourceIcon { glow::system::resource_icon() };
    auto systemIcon { glow::system::system_icon() };
    auto systemCursor { glow::system::system_cursor() };
    auto systemBrush { glow::system::system_brush() };

    ::WNDCLASSEXW windowClass { .cbSize { sizeof(::WNDCLASSEXW) },
                                .style { 0 },
                                .lpfnWndProc { procedure },
                                .cbClsExtra { 0 },
                                .cbWndExtra { sizeof(Window) },
                                .hInstance { instance },
                                .hIcon { resourceIcon ? resourceIcon : systemIcon },
                                .hCursor { systemCursor },
                                .hbrBackground { systemBrush },
                                .lpszMenuName { nullptr },
                                .lpszClassName { className.c_str() },
                                .hIconSm { resourceIcon ? resourceIcon : systemIcon } };

    if (::GetClassInfoExW(instance, className.c_str(), &windowClass) == 0) {
        ::RegisterClassExW(&windowClass);
    }

    ::CreateWindowExW(0,
                      className.c_str(),
                      glow::text::to_wstring(title).c_str(),
                      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      nullptr,
                      nullptr,
                      instance,
                      this);
}

auto CALLBACK Window::procedure(::HWND hwnd,
                                ::UINT msg,
                                ::WPARAM wparam,
                                ::LPARAM lparam) -> ::LRESULT {
    if (msg == WM_NCCREATE) {
        auto create { reinterpret_cast<::CREATESTRUCTW*>(lparam) };

        if (auto self { static_cast<Window*>(create->lpCreateParams) }; self) {
            ::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<::LONG_PTR>(self));
            self->hwnd.reset(hwnd);
            self->dpi = static_cast<uint32_t>(::GetDpiForWindow(hwnd));
            self->scale
                = (static_cast<double>(self->dpi) / static_cast<double>(USER_DEFAULT_SCREEN_DPI));
        }
    }

    if (auto self { reinterpret_cast<Window*>(::GetWindowLongPtrW(hwnd, 0)) }; self) {
        if (msg == WM_NCDESTROY) {
            ::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<::LONG_PTR>(nullptr));
        }

        if (msg == WM_WINDOWPOSCHANGED) {
            auto windowPos { reinterpret_cast<::LPWINDOWPOS>(lparam) };

            self->window.x = windowPos->x;
            self->window.y = windowPos->y;
            self->window.width = windowPos->cx;
            self->window.height = windowPos->cy;

            ::GetWindowPlacement(hwnd, &self->placement);

            ::RECT rect {};
            ::GetClientRect(hwnd, &rect);

            self->client.x = rect.left;
            self->client.y = rect.top;
            self->client.width = rect.right - rect.left;
            self->client.height = rect.bottom - rect.top;

            ::MONITORINFO mi { sizeof(::MONITORINFO) };
            ::GetMonitorInfoW(::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi);

            self->monitor.x = mi.rcWork.left;
            self->monitor.y = mi.rcWork.top;
            self->monitor.width = mi.rcWork.right - mi.rcWork.left;
            self->monitor.height = mi.rcWork.bottom - mi.rcWork.top;
        }

        if (msg == WM_DPICHANGED) {
            auto rect { reinterpret_cast<::LPRECT>(lparam) };

            ::SetWindowPos(hwnd,
                           nullptr,
                           rect->left,
                           rect->top,
                           (rect->right - rect->left),
                           (rect->bottom - rect->top),
                           SWP_NOZORDER | SWP_NOACTIVATE);

            self->dpi = static_cast<uint32_t>(::GetDpiForWindow(hwnd));
            self->scale
                = (static_cast<double>(self->dpi) / static_cast<double>(USER_DEFAULT_SCREEN_DPI));
        }

        if (self->message.contains(msg)) {
            return self->message.invoke({ hwnd, msg, wparam, lparam });
        }

        if (msg == WM_CLOSE) {
            self->hwnd.reset();

            return 0;
        }
    }

    return ::DefWindowProcW(hwnd, msg, wparam, lparam);
}

auto Window::activate() -> void { ::ShowWindow(hwnd.get(), SW_NORMAL); }

auto Window::show() -> void { ::ShowWindow(hwnd.get(), SW_SHOW); }

auto Window::hide() -> void { ::ShowWindow(hwnd.get(), SW_HIDE); }

auto Window::is_visible() -> bool { return ::IsWindowVisible(hwnd.get()); }

auto Window::set_position(Position position) -> void {
    ::SetWindowPos(hwnd.get(),
                   nullptr,
                   position.x,
                   position.y,
                   position.width,
                   position.height,
                   SWP_NOZORDER | SWP_NOACTIVATE);
}

auto Window::set_style(::LONG_PTR style) -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, style);
}

auto Window::get_style() -> ::LONG_PTR { ::GetWindowLongPtrW(hwnd.get(), GWL_STYLE); }

auto Window::toggle_centered(bool centered) -> void {
    restore = window;

    if (centered) {
        if (monitor.width > window.width && monitor.height > window.height) {
            auto x { static_cast<int>((monitor.width - window.width) / 2) };
            auto y { static_cast<int>((monitor.height - window.height) / 2) };

            ::SetWindowPos(
                hwnd.get(), nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
        }
    } else {
        set_position(restore);
    }
}

auto Window::toggle_topmost(bool topmost) -> void {
    ::SetWindowPos(
        hwnd.get(), topmost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

auto Window::timer_start(::UINT_PTR timerId, ::UINT intervalMs) -> bool {
    return ::SetTimer(hwnd.get(), timerId, intervalMs, nullptr) != 0 ? true : false;
}

auto Window::timer_stop(::UINT_PTR timerId) -> bool { return ::KillTimer(hwnd.get(), timerId); }
} // namespace glow

namespace glow::window {
// auto register_class(::WNDCLASSEXA* windowClass) -> void {
//     if (::GetClassInfoExA(glow::system::get_instance(), windowClass->lpszClassName, windowClass)
//         == 0) {
//         ::RegisterClassExA(windowClass);
//     }
// }

// auto get_class_name(::HWND hwnd) -> std::string {
//     std::string buffer(256, 0);
//     ::GetClassNameA(hwnd, buffer.data(), glow::math::check_safe_size<int>(buffer.size()));
//     return buffer;
// }

// auto find_message_only(const std::string& name) -> ::HWND {
//     return ::FindWindowExA(HWND_MESSAGE, nullptr, name.c_str(), nullptr);
// }

// auto set_title(::HWND hwnd, std::string title) -> bool {
//     return ::SetWindowTextA(hwnd, title.c_str());
// }

// auto activate(::HWND hwnd) -> bool { return ::ShowWindow(hwnd, SW_NORMAL); }

// auto show(::HWND hwnd) -> bool { return ::ShowWindow(hwnd, SW_SHOW); }

// auto hide(::HWND hwnd) -> bool { return ::ShowWindow(hwnd, SW_HIDE); }

// auto maximize(::HWND hwnd) -> bool { return ::ShowWindow(hwnd, SW_MAXIMIZE); }

// auto minimize(::HWND hwnd) -> bool { return ::ShowWindow(hwnd, SW_MINIMIZE); }

// auto restore(::HWND hwnd) -> bool { return ::ShowWindow(hwnd, SW_RESTORE); }

// auto check_visibility(::HWND hwnd) -> bool { return ::IsWindowVisible(hwnd); }

// auto check_maximize(::HWND hwnd) -> bool { return ::IsZoomed(hwnd); }

// auto toggle_maximize(::HWND hwnd) -> bool {
//     if (check_maximize(hwnd)) {
//         return restore(hwnd);
//     } else {
//         return maximize(hwnd);
//     }
// }

// auto toggle_fullscreen(::HWND hwnd) -> void {
//     thread_local ::RECT pos { .left { 0 }, .top { 0 }, .right { 0 }, .bottom { 0 } };

//     if (check_overlapped(hwnd)) {
//         ::MONITORINFO mi {
//             .cbSize { sizeof(::MONITORINFO) },
//             .rcMonitor { ::RECT { .left { 0 }, .top { 0 }, .right { 0 }, .bottom { 0 } } },
//             .rcWork { ::RECT { .left { 0 }, .top { 0 }, .right { 0 }, .bottom { 0 } } },
//             .dwFlags { 0 }
//         };
//         ::GetWindowRect(hwnd, &pos);
//         if (::GetMonitorInfoA(::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi)) {
//             ::SetWindowLongPtrA(hwnd, GWL_STYLE, get_style(hwnd) & ~WS_OVERLAPPEDWINDOW);
//             ::SetWindowPos(hwnd,
//                            HWND_TOP,
//                            mi.rcMonitor.left,
//                            mi.rcMonitor.top,
//                            mi.rcMonitor.right - mi.rcMonitor.left,
//                            mi.rcMonitor.bottom - mi.rcMonitor.top,
//                            SWP_FRAMECHANGED);
//         }
//     } else {
//         ::SetWindowLongPtrA(hwnd, GWL_STYLE, get_style(hwnd) | WS_OVERLAPPEDWINDOW);
//         ::SetWindowPos(hwnd,
//                        nullptr,
//                        pos.left,
//                        pos.top,
//                        (pos.right - pos.left),
//                        (pos.bottom - pos.top),
//                        SWP_FRAMECHANGED);
//     }
// }

// auto set_position(::HWND hwnd, int x, int y, int cx, int cy) -> bool {
//     return ::SetWindowPos(hwnd, nullptr, x, y, cx, cy, 0);
// }

// auto top(::HWND hwnd) -> bool {
//     return ::SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
// }

// auto bottom(::HWND hwnd) -> bool {
//     return ::SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
// }

// auto topmost(::HWND hwnd) -> bool {
//     return ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
// }

// auto no_topmost(::HWND hwnd) -> bool {
//     return ::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
// }

// auto check_topmost(::HWND hwnd) -> bool { return get_ex_style(hwnd) & WS_EX_TOPMOST; }

// auto toggle_topmost(::HWND hwnd) -> bool {
//     flash(hwnd);

//     if (check_topmost(hwnd)) {
//         return no_topmost(hwnd);
//     } else {
//         return topmost(hwnd);
//     }
// }

// auto flash(::HWND hwnd) -> bool {
//     ::FLASHWINFO fwi { .cbSize { sizeof(::FLASHWINFO) },
//                        .hwnd { hwnd },
//                        .dwFlags { FLASHW_CAPTION },
//                        .uCount { 1 },
//                        .dwTimeout { 100 } };
//     return ::FlashWindowEx(&fwi);
// }

// auto cloak(::HWND hwnd) -> void {
//     auto cloak { TRUE };
//     ::DwmSetWindowAttribute(hwnd, ::DWMWINDOWATTRIBUTE::DWMWA_CLOAK, &cloak, sizeof(cloak));
// }

// auto uncloak(::HWND hwnd) -> void {
//     auto cloak { FALSE };
//     ::DwmSetWindowAttribute(hwnd, ::DWMWINDOWATTRIBUTE::DWMWA_CLOAK, &cloak, sizeof(cloak));
// }

// auto check_cloak(::HWND hwnd) -> bool {
//     auto cloaked { DWM_CLOAKED_APP };
//     ::DwmGetWindowAttribute(hwnd, ::DWMWINDOWATTRIBUTE::DWMWA_CLOAKED, &cloaked,
//     sizeof(cloaked));

//     switch (cloaked) {
//         case DWM_CLOAKED_APP:
//             return true;
//         case DWM_CLOAKED_INHERITED:
//             return true;
//         case DWM_CLOAKED_SHELL:
//             return true;
//     }

//     return false;
// }

// auto enable_dark_mode(::HWND hwnd) -> void {
//     auto attribute { TRUE };
//     ::DwmSetWindowAttribute(
//         hwnd, ::DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &attribute,
//         sizeof(attribute));
// }

// auto disable_dark_mode(::HWND hwnd) -> void {
//     auto attribute { FALSE };
//     ::DwmSetWindowAttribute(
//         hwnd, ::DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &attribute,
//         sizeof(attribute));
// }

// auto set_backdrop(::HWND hwnd, ::DWM_SYSTEMBACKDROP_TYPE backdrop) -> void {
//     ::MARGINS margins { .cxLeftWidth { 0 },
//                         .cxRightWidth { 0 },
//                         .cyTopHeight { 0 },
//                         .cyBottomHeight { ::GetSystemMetrics(SM_CYVIRTUALSCREEN) } };

//     ::DwmExtendFrameIntoClientArea(hwnd, &margins);
//     ::DwmSetWindowAttribute(
//         hwnd, ::DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop));
// }

// auto set_round_corners(::HWND hwnd, ::DWM_WINDOW_CORNER_PREFERENCE corner) -> void {
//     ::DwmSetWindowAttribute(
//         hwnd, ::DWMWINDOWATTRIBUTE::DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
// }

// auto set_caption_color(::HWND hwnd, ::COLORREF color) -> void {
//     ::DwmSetWindowAttribute(hwnd, ::DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR, &color,
//     sizeof(color));
// }

// auto set_border_color(::HWND hwnd, ::COLORREF color) -> void {
//     ::DwmSetWindowAttribute(hwnd, ::DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, &color,
//     sizeof(color));
// }

// auto set_text_color(::HWND hwnd, ::COLORREF color) -> void {
//     ::DwmSetWindowAttribute(hwnd, ::DWMWINDOWATTRIBUTE::DWMWA_TEXT_COLOR, &color, sizeof(color));
// }

// auto set_focus(::HWND hwnd) -> ::HWND { return ::SetFocus(hwnd); }

// auto get_focus() -> ::HWND { return ::GetFocus(); }

// auto set_foreground(::HWND hwnd) -> bool { return ::SetForegroundWindow(hwnd); }

// auto get_foreground() -> ::HWND { return ::GetForegroundWindow(); }

// auto set_active(::HWND hwnd) -> ::HWND { return ::SetActiveWindow(hwnd); }

// auto get_active() -> ::HWND { return ::GetActiveWindow(); }

// auto set_parent(::HWND hwnd, ::HWND parent) -> ::HWND { return ::SetParent(hwnd, parent); }

// auto get_parent(::HWND hwnd) -> ::HWND { return ::GetParent(hwnd); }

// auto bring_to_top(::HWND hwnd) -> bool { return ::BringWindowToTop(hwnd); }

// auto close(::HWND hwnd) -> ::LRESULT { return ::SendMessageA(hwnd, WM_CLOSE, 0, 0); }

// auto destroy(::HWND hwnd) -> bool { return ::DestroyWindow(hwnd); }

// auto get_current_dpi(::HWND hwnd) -> ::UINT { return ::GetDpiForWindow(hwnd); }

// auto get_current_scale(::HWND hwnd) -> double {
//     return static_cast<double>(::GetDpiForWindow(hwnd))
//         / static_cast<double>(USER_DEFAULT_SCREEN_DPI);
// }

// auto center(::HWND hwnd) -> void {
//     ::MONITORINFO mi { .cbSize { sizeof(::MONITORINFO) },
//                        .rcMonitor {
//                            ::RECT { .left { 0 }, .top { 0 }, .right { 0 }, .bottom { 0 } } },
//                        .rcWork { ::RECT { .left { 0 }, .top { 0 }, .right { 0 }, .bottom { 0 } }
//                        }, .dwFlags { 0 } };

//     if (::GetMonitorInfoA(::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi)) {
//         ::RECT windowRect { .left { 0 }, .top { 0 }, .right { 0 }, .bottom { 0 } };
//         ::GetWindowRect(hwnd, &windowRect);

//         auto width { static_cast<int>(windowRect.right - windowRect.left) };
//         auto height { static_cast<int>(windowRect.bottom - windowRect.top) };

//         auto x { (static_cast<int>(mi.rcWork.right - mi.rcWork.left) - width) / 2 };
//         auto y { (static_cast<int>(mi.rcWork.bottom - mi.rcWork.top) - height) / 2 };

//         ::SetWindowPos(hwnd, nullptr, x, y, width, height, 0);
//     }
// }

// auto set_placement(::HWND hwnd, const ::WINDOWPLACEMENT* windowPlacement) -> bool {
//     return ::SetWindowPlacement(hwnd, windowPlacement);
// }

// auto get_placement(::HWND hwnd, ::WINDOWPLACEMENT* windowPlacement) -> bool {
//     return ::GetWindowPlacement(hwnd, windowPlacement);
// }

// auto get_client_rect(::HWND hwnd) -> ::RECT {
//     ::RECT clientRect { .left { 0 }, .top { 0 }, .right { 0 }, .bottom { 0 } };
//     ::GetClientRect(hwnd, &clientRect);
//     return clientRect;
// }

// auto get_window_rect(::HWND hwnd) -> ::RECT {
//     ::RECT windowRect { .left { 0 }, .top { 0 }, .right { 0 }, .bottom { 0 } };
//     ::GetWindowRect(hwnd, &windowRect);
//     return windowRect;
// }

// auto set_style(::HWND hwnd, ::DWORD style) -> ::DWORD {
//     return static_cast<::DWORD>(::SetWindowLongPtrA(hwnd, GWL_STYLE, style));
// }

// auto get_style(::HWND hwnd) -> ::DWORD {
//     return static_cast<::DWORD>(::GetWindowLongPtrA(hwnd, GWL_STYLE));
// }

// auto set_ex_style(::HWND hwnd, ::DWORD exStyle) -> ::DWORD {
//     return static_cast<::DWORD>(::SetWindowLongPtrA(hwnd, GWL_EXSTYLE, exStyle));
// }

// auto get_ex_style(::HWND hwnd) -> ::DWORD {
//     return static_cast<::DWORD>(::GetWindowLongPtrA(hwnd, GWL_EXSTYLE));
// }

// auto get_id(::HWND hwnd) -> uintptr_t { return ::GetWindowLongPtrA(hwnd, GWLP_ID); }

// auto set_overlapped(::HWND hwnd) -> void {
//     ::SetWindowLongPtrA(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
//     ::SetWindowPos(hwnd,
//                    nullptr,
//                    0,
//                    0,
//                    0,
//                    0,
//                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
// }

// auto check_overlapped(::HWND hwnd) -> bool { return get_style(hwnd) & WS_OVERLAPPEDWINDOW; }

// auto set_child(::HWND hwnd) -> void {
//     ::SetWindowLongPtrA(hwnd, GWL_STYLE, WS_CHILDWINDOW | WS_CLIPSIBLINGS);
//     ::SetWindowPos(hwnd,
//                    nullptr,
//                    0,
//                    0,
//                    0,
//                    0,
//                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
// }

// auto check_child(::HWND hwnd) -> bool { return get_style(hwnd) & WS_CHILDWINDOW; }

// auto set_small_icon(::HWND hwnd, ::HICON hicon) -> void {
//     ::SendMessageA(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<::LPARAM>(hicon));
// }

// auto set_big_icon(::HWND hwnd, ::HICON hicon) -> void {
//     ::SendMessageA(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<::LPARAM>(hicon));
// }

// auto add_border(::HWND hwnd) -> void {
//     ::SetWindowLongPtrA(hwnd, GWL_STYLE, get_style(hwnd) | WS_BORDER);
//     ::SetWindowPos(hwnd,
//                    nullptr,
//                    0,
//                    0,
//                    0,
//                    0,
//                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
// }

// auto remove_border(::HWND hwnd) -> void {
//     ::SetWindowLongPtrA(hwnd, GWL_STYLE, get_style(hwnd) & ~WS_BORDER);
//     ::SetWindowPos(hwnd,
//                    nullptr,
//                    0,
//                    0,
//                    0,
//                    0,
//                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
// }

// auto check_dark() -> bool {
//     auto uiSettings { winrt::Windows::UI::ViewManagement::UISettings() };
//     auto foregroundColor { uiSettings.GetColorValue(
//         winrt::Windows::UI::ViewManagement::UIColorType::Foreground) };

//     if (((5 * foregroundColor.G) + (2 * foregroundColor.R) + foregroundColor.B) > (8 * 128)) {
//         return true;
//     } else {
//         return false;
//     }
// }

// auto def_window_proc(::HWND hwnd, ::UINT msg, ::WPARAM wparam, ::LPARAM lparam) -> ::LRESULT {
//     return ::DefWindowProcA(hwnd, msg, wparam, lparam);
// }

// auto CALLBACK window_proc(::HWND hwnd, ::UINT msg, ::WPARAM wparam, ::LPARAM lparam) -> ::LRESULT
// {
//     switch (msg) {
//         case WM_NCCREATE: {
//             auto create { reinterpret_cast<::CREATESTRUCTA*>(lparam) };

//             if (auto self { static_cast<Window*>(create->lpCreateParams) }; self) {
//                 ::SetWindowLongPtrA(hwnd, 0, reinterpret_cast<::LONG_PTR>(self));
//                 self->m_hwnd.reset(hwnd);
//                 glow::window::get_placement(hwnd, &self->m_windowPlacement);
//             }
//         } break;
//         case WM_NCDESTROY: {
//             if (auto self { reinterpret_cast<Window*>(::GetWindowLongPtrA(hwnd, 0)) }; self) {
//                 ::SetWindowLongPtrA(hwnd, 0, reinterpret_cast<::LONG_PTR>(nullptr));
//             }
//         } break;
//     }

//     if (auto self { reinterpret_cast<Window*>(::GetWindowLongPtrA(hwnd, 0)) }; self) {
//         if (self->message(msg)) {
//             return self->message({ hwnd, msg, wparam, lparam });
//         } else {
//             switch (msg) {
//                 case WM_CLOSE: {
//                     self->m_hwnd.reset();
//                     return 0;
//                 } break;
//             }
//         }
//     }

//     return def_window_proc(hwnd, msg, wparam, lparam);
// }

// auto Window::create(const std::string& title, bool visible) -> void {
//     m_windowClass.lpszClassName = "OverlappedWindow";

//     register_class(&m_windowClass);

//     if (::CreateWindowExA(0,
//                           m_windowClass.lpszClassName,
//                           title.empty() ? m_windowClass.lpszClassName : title.c_str(),
//                           WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
//                           CW_USEDEFAULT,
//                           CW_USEDEFAULT,
//                           CW_USEDEFAULT,
//                           CW_USEDEFAULT,
//                           nullptr,
//                           nullptr,
//                           glow::system::get_instance(),
//                           this)
//         == 0) {
//         throw std::runtime_error(glow::log::get_last_error());
//     }

//     if (visible) {
//         glow::window::activate(m_hwnd.get());
//     }
// }

// auto Window::create(::HWND parent, const std::string& title, bool visible) -> void {
//     m_windowClass.lpszClassName = "ChildWindow";

//     register_class(&m_windowClass);

//     if (::CreateWindowExA(0,
//                           m_windowClass.lpszClassName,
//                           title.empty() ? m_windowClass.lpszClassName : title.c_str(),
//                           WS_CHILDWINDOW | WS_CLIPSIBLINGS,
//                           CW_USEDEFAULT,
//                           CW_USEDEFAULT,
//                           CW_USEDEFAULT,
//                           CW_USEDEFAULT,
//                           parent,
//                           reinterpret_cast<::HMENU>(m_id),
//                           glow::system::get_instance(),
//                           this)
//         == 0) {
//         throw std::runtime_error(glow::log::get_last_error());
//     }

//     if (visible) {
//         glow::window::activate(m_hwnd.get());
//     }
// }

// auto Window::create_message_only() -> void {
//     m_windowClass.lpszClassName = "MessageWindow";

//     register_class(&m_windowClass);

//     if (::CreateWindowExA(0,
//                           m_windowClass.lpszClassName,
//                           m_windowClass.lpszClassName,
//                           0,
//                           CW_USEDEFAULT,
//                           CW_USEDEFAULT,
//                           CW_USEDEFAULT,
//                           CW_USEDEFAULT,
//                           HWND_MESSAGE,
//                           nullptr,
//                           glow::system::get_instance(),
//                           this)
//         == 0) {
//         throw std::runtime_error(glow::log::get_last_error());
//     }
// }

// auto Window::close() -> void { m_hwnd.reset(); }

// auto Window::message(::UINT msg,
//                      std::function<::LRESULT(glow::message::wm message)> callback) -> bool {
//     auto emplace { m_map.try_emplace(msg, callback) };
//     return emplace.second;
// }

// auto Window::message(::UINT msg) -> bool { return m_map.contains(msg); }

// auto Window::message(glow::message::wm message) -> ::LRESULT {
//     return m_map.find(message.msg)
//         ->second({ message.hwnd, message.msg, message.wparam, message.lparam });
// }

// auto Window::notify(glow::message::notice notice,
//                     const std::string& message,
//                     const std::string& receiver) -> void {
//     glow::message::Notification notification { .nmhdr { .hwndFrom { m_hwnd.get() },
//                                                         .idFrom { m_id },
//                                                         .code { std::to_underlying(notice) } },
//                                                .hwndFrom { m_hwnd.get() },
//                                                .idFrom { m_id },
//                                                .notice { notice },
//                                                .message { message } };

//     glow::message::send({ glow::window::find_message_only(receiver),
//                           WM_NOTIFY,
//                           notification.nmhdr.idFrom,
//                           reinterpret_cast<::LPARAM>(&notification) });
// }

// auto Window::notify(::HWND receiver,
//                     glow::message::notice notice,
//                     const std::string& message) -> void {
//     glow::message::Notification notification { .nmhdr { .hwndFrom { m_hwnd.get() },
//                                                         .idFrom { m_id },
//                                                         .code { std::to_underlying(notice) } },
//                                                .hwndFrom { m_hwnd.get() },
//                                                .idFrom { m_id },
//                                                .notice { notice },
//                                                .message { message } };

//     glow::message::send({ receiver,
//                           WM_NOTIFY,
//                           notification.nmhdr.idFrom,
//                           reinterpret_cast<::LPARAM>(&notification) });
// }
}; // namespace glow::window
