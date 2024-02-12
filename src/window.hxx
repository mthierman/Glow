// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <dwmapi.h>

#ifdef small
#undef small
#endif

#include <stdexcept>
#include <string>

#include <wil/resource.h>

#include "gui.hxx"
#include "notification.hxx"
#include "position.hxx"
#include "random.hxx"

namespace glow
{
template <typename T> struct Window
{
    Window(std::string name = "Window", intptr_t id = glow::random<intptr_t>(),
           ::DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, ::DWORD exStyle = 0,
           int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = CW_USEDEFAULT,
           int height = CW_USEDEFAULT, ::HWND parent = nullptr, ::HMENU menu = nullptr)
        : m_id{id}
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
            wcex.hbrBackground = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
            wcex.hCursor = static_cast<HCURSOR>(
                ::LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
            wcex.hIcon = m_hicon.get()
                             ? m_hicon.get()
                             : static_cast<HICON>(::LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON,
                                                               0, 0, LR_SHARED | LR_DEFAULTSIZE));
            wcex.hIconSm =
                m_hicon.get()
                    ? m_hicon.get()
                    : static_cast<HICON>(::LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                                      LR_SHARED | LR_DEFAULTSIZE));

            if (::RegisterClassExA(&wcex) == 0)
            {
                throw std::runtime_error("Class registration failure");
            }
        }

        if (::CreateWindowExA(exStyle, wcex.lpszClassName, wcex.lpszClassName, style, x, y, width,
                              height, parent, menu, ::GetModuleHandleA(nullptr), this) == nullptr)
        {
            throw std::runtime_error("Window creation failure");
        }

        dpi();
        scale();
    }

    virtual ~Window() {}

    auto hwnd() const -> ::HWND { return m_hwnd.get(); }

    auto hicon() const -> ::HICON { return m_hicon.get(); }

    auto id() const -> intptr_t { return m_id; }

    auto close() -> int
    {
        m_hwnd.reset();

        return 0;
    }

    auto notify(::HWND receiver, CODE code, std::string message = "") -> void
    {
        m_notification.nmhdr.hwndFrom = hwnd();
        m_notification.hwnd = hwnd();

        m_notification.nmhdr.idFrom = id();
        m_notification.id = id();

        m_notification.nmhdr.code = std::to_underlying(code);
        m_notification.code = code;

        m_notification.message = message;

        ::SendMessageA(receiver, WM_NOTIFY, reinterpret_cast<intptr_t>(&m_notification.id),
                       reinterpret_cast<intptr_t>(&m_notification));
    }

    auto dpi() -> void { m_dpi = ::GetDpiForWindow(hwnd()); };

    auto scale() -> void
    {
        m_scale = static_cast<float>(m_dpi) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
    };

    auto reveal() -> void { ::ShowWindow(hwnd(), SW_SHOWNORMAL); }

    auto show() -> void { ::ShowWindow(hwnd(), SW_SHOW); }

    auto hide() -> void { ::ShowWindow(hwnd(), SW_HIDE); }

    auto is_visible() -> bool { return ::IsWindowVisible(hwnd()); }

    auto focus() -> void { ::SetFocus(hwnd()); }

    auto is_focus() -> bool { ::GetFocus() == hwnd(); }

    auto foreground() -> void { ::SetForegroundWindow(hwnd()); }

    auto is_foreground() -> bool { return ::GetForegroundWindow() == hwnd(); }

    auto active() -> void { ::SetActiveWindow(hwnd()); }

    auto top() -> void { ::BringWindowToTop(hwnd()); }

    auto maximize() -> void
    {
        auto style{::GetWindowLongPtrA(hwnd(), GWL_STYLE)};

        ::WINDOWPLACEMENT wp{sizeof(::WINDOWPLACEMENT)};
        ::GetWindowPlacement(hwnd(), &wp);

        if ((style & WS_OVERLAPPEDWINDOW) && wp.showCmd == 3) { ::ShowWindow(hwnd(), SW_RESTORE); }

        else { ::ShowWindow(hwnd(), SW_MAXIMIZE); }
    }

    auto fullscreen() -> void
    {
        static RECT pos;

        auto style{::GetWindowLongPtrA(hwnd(), GWL_STYLE)};

        if (style & WS_OVERLAPPEDWINDOW)
        {
            ::MONITORINFO mi{sizeof(::MONITORINFO)};
            ::GetWindowRect(hwnd(), &pos);
            if (::GetMonitorInfoA(::MonitorFromWindow(hwnd(), MONITOR_DEFAULTTONEAREST), &mi))
            {
                ::SetWindowLongPtrA(hwnd(), GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
                ::SetWindowPos(hwnd(), HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                               mi.rcMonitor.right - mi.rcMonitor.left,
                               mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED);
            }
        }

        else
        {
            ::SetWindowLongPtrA(hwnd(), GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
            ::SetWindowPos(hwnd(), nullptr, pos.left, pos.top, (pos.right - pos.left),
                           (pos.bottom - pos.top), SWP_FRAMECHANGED);
        }
    }

    auto topmost() -> void
    {
        ::FLASHWINFO fwi{sizeof(::FLASHWINFO)};
        fwi.hwnd = hwnd();
        fwi.dwFlags = FLASHW_CAPTION;
        fwi.uCount = 1;
        fwi.dwTimeout = 100;

        auto style{::GetWindowLongPtrA(hwnd(), GWL_EXSTYLE)};

        if (style & WS_EX_TOPMOST)
        {
            ::SetWindowPos(hwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }

        else { ::SetWindowPos(hwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); }

        ::FlashWindowEx(&fwi);
    }

    auto title(std::string title) -> void { ::SetWindowTextA(hwnd(), title.c_str()); }

    auto icon(::HICON icon, bool small = true, bool big = true) -> void
    {
        if (small)
        {
            ::SendMessageA(hwnd(), WM_SETICON, ICON_SMALL, reinterpret_cast<::LPARAM>(icon));
        }

        if (big) { ::SendMessageA(hwnd(), WM_SETICON, ICON_BIG, reinterpret_cast<::LPARAM>(icon)); }
    }

    auto border(bool enabled) -> void
    {
        auto style{::GetWindowLongPtrA(hwnd(), GWL_STYLE)};

        ::SetWindowLongPtrA(hwnd(), GWL_STYLE,
                            enabled ? (style | WS_BORDER) : (style & ~WS_BORDER));
        ::SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto overlapped() -> void
    {
        ::SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
        ::SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto popup() -> void
    {
        ::SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
        ::SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto child() -> void
    {
        ::SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_CHILD);
        ::SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto reparent(::HWND parent) -> void
    {
        if (parent)
        {
            ::SetParent(hwnd(), parent);
            child();
        }

        else
        {
            ::SetParent(hwnd(), nullptr);
            popup();
        }
    }

    auto position() -> void
    {
        ::GetClientRect(hwnd(), &m_client.rect);

        m_client.x = m_client.rect.left;
        m_client.y = m_client.rect.top;
        m_client.width = m_client.rect.right - m_client.rect.left;
        m_client.height = m_client.rect.bottom - m_client.rect.top;

        ::GetWindowRect(hwnd(), &m_window.rect);

        m_window.x = m_window.rect.left;
        m_window.y = m_window.rect.top;
        m_window.width = m_window.rect.right - m_window.rect.left;
        m_window.height = m_window.rect.bottom - m_window.rect.top;
    }

    auto size() -> void { ::SendMessageA(hwnd(), WM_SIZE, 0, 0); }

    auto theme() -> void
    {
        if (glow::check_theme()) { dwm_dark_mode(true); }

        else { dwm_dark_mode(false); }
    }

    auto dwm_dark_mode(bool enabled) -> void
    {
        if (enabled)
        {
            auto useImmersiveDarkMode{TRUE};
            ::DwmSetWindowAttribute(hwnd(), ::DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
                                    sizeof(useImmersiveDarkMode));
        }

        else
        {
            auto useImmersiveDarkMode{FALSE};
            ::DwmSetWindowAttribute(hwnd(), ::DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
                                    sizeof(useImmersiveDarkMode));
        }
    }

    auto dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE backdrop) -> void
    {
        // MARGINS m{-1};
        ::MARGINS m{0, 0, 0, ::GetSystemMetrics(SM_CYVIRTUALSCREEN)};
        if (FAILED(::DwmExtendFrameIntoClientArea(m_hwnd.get(), &m))) { return; }

        if (FAILED(::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_SYSTEMBACKDROP_TYPE, &backdrop,
                                           sizeof(&backdrop))))
        {
            return;
        }
    }

    auto dwm_rounded_corners(bool enable) -> void
    {
        if (enable)
        {
            auto corner{::DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_ROUND};
            ::DwmSetWindowAttribute(hwnd(), ::DWMWA_WINDOW_CORNER_PREFERENCE, &corner,
                                    sizeof(corner));
        }

        else
        {
            auto corner{::DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DONOTROUND};
            ::DwmSetWindowAttribute(hwnd(), ::DWMWA_WINDOW_CORNER_PREFERENCE, &corner,
                                    sizeof(corner));
        }
    }

    auto dwm_cloak(bool enable) -> void
    {
        if (enable)
        {
            auto cloak{TRUE};
            ::DwmSetWindowAttribute(hwnd(), ::DWMWA_CLOAK, &cloak, sizeof(cloak));
        }

        else
        {
            auto cloak{FALSE};
            ::DwmSetWindowAttribute(hwnd(), ::DWMWA_CLOAK, &cloak, sizeof(cloak));
        }
    }

    auto dwm_caption_color(bool enable) -> void
    {
        if (enable)
        {
            auto captionColor{DWMWA_COLOR_DEFAULT};
            ::DwmSetWindowAttribute(hwnd(), ::DWMWA_CAPTION_COLOR, &captionColor,
                                    sizeof(captionColor));
        }

        else
        {
            auto captionColor{DWMWA_COLOR_NONE};
            ::DwmSetWindowAttribute(hwnd(), ::DWMWA_CAPTION_COLOR, &captionColor,
                                    sizeof(captionColor));
        }
    }

    auto dwm_set_caption_color(::COLORREF color) -> void
    {
        ::DwmSetWindowAttribute(hwnd(), ::DWMWA_CAPTION_COLOR, &color, sizeof(color));
    }

    auto dwm_border_color(bool enable) -> void
    {
        if (enable)
        {
            auto borderColor{DWMWA_COLOR_DEFAULT};
            ::DwmSetWindowAttribute(hwnd(), ::DWMWA_BORDER_COLOR, &borderColor,
                                    sizeof(borderColor));
        }

        else
        {
            auto borderColor{DWMWA_COLOR_NONE};
            ::DwmSetWindowAttribute(hwnd(), ::DWMWA_BORDER_COLOR, &borderColor,
                                    sizeof(borderColor));
        }
    }

    auto dwm_set_border_color(::COLORREF color) -> void
    {
        ::DwmSetWindowAttribute(hwnd(), ::DWMWA_BORDER_COLOR, &color, sizeof(color));
    }

    auto dwm_set_text_color(::COLORREF color) -> void
    {
        ::DwmSetWindowAttribute(hwnd(), ::DWMWA_TEXT_COLOR, &color, sizeof(color));
    }

    auto dwm_reset_text_color() -> void
    {
        auto textColor{DWMWA_COLOR_DEFAULT};
        ::DwmSetWindowAttribute(hwnd(), ::DWMWA_TEXT_COLOR, &textColor, sizeof(textColor));
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

        else self = reinterpret_cast<T*>(::GetWindowLongPtrA(hwnd, 0));

        if (self) { return self->default_wnd_proc(hwnd, message, wParam, lParam); }

        else { return ::DefWindowProcA(hwnd, message, wParam, lParam); }
    }

    virtual auto default_wnd_proc(::HWND hwnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT
    {
        switch (message)
        {
        case WM_CLOSE: return close();
        }

        return wnd_proc(hwnd, message, wParam, lParam);
    }

    virtual auto wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        return ::DefWindowProcA(hwnd, message, wParam, lParam);
    }

  public:
    intptr_t m_id{};
    wil::unique_hwnd m_hwnd{};
    wil::unique_hicon m_hicon{static_cast<HICON>(::LoadImageA(
        ::GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};

    glow::Notification m_notification{};
    glow::Position m_client{};
    glow::Position m_window{};
    int m_dpi{};
    float m_scale{};
    bool m_maximize{};
    bool m_fullscreen{};
    bool m_topmost{};
};
} // namespace glow
