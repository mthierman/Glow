// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "window.hxx"

namespace glow
{
Window::Window(std::string name, size_t id, ::DWORD style, ::DWORD exStyle, int x, int y, int width,
               int height, ::HWND parent, ::HMENU menu)
    : m_id{id}
{
    m_appIcon.reset(static_cast<HICON>(
        ::LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE)));

    m_hicon.reset(static_cast<HICON>(::LoadImageA(::GetModuleHandleA(nullptr), MAKEINTRESOURCEA(1),
                                                  IMAGE_ICON, 0, 0, LR_DEFAULTSIZE)));

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
        wcex.hIcon = m_hicon.get() ? m_hicon.get() : m_appIcon.get();
        wcex.hIconSm = m_hicon.get() ? m_hicon.get() : m_appIcon.get();

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
}

Window::~Window() {}

auto Window::notify(::HWND receiver, CODE code, std::string message) -> void
{
    m_notification.reset();

    m_notification.nmhdr.hwndFrom = m_hwnd.get();
    m_notification.hwnd = m_hwnd.get();

    m_notification.nmhdr.idFrom = m_id;
    m_notification.id = m_id;

    m_notification.nmhdr.code = std::to_underlying(code);
    m_notification.code = code;

    m_notification.message = message;

    ::SendMessageA(receiver, WM_NOTIFY, reinterpret_cast<uintptr_t>(&m_notification.id),
                   reinterpret_cast<uintptr_t>(&m_notification));
}

auto Window::get_dpi() -> unsigned int { return ::GetDpiForWindow(m_hwnd.get()); };

auto Window::get_scale() -> float
{
    return static_cast<float>(m_dpi) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
};

auto Window::get_style() -> intptr_t { return ::GetWindowLongPtrA(m_hwnd.get(), GWL_STYLE); }

auto Window::get_ex_style() -> intptr_t { return ::GetWindowLongPtrA(m_hwnd.get(), GWL_EXSTYLE); }

auto Window::get_id() -> intptr_t { return ::GetWindowLongPtrA(m_hwnd.get(), GWLP_ID); }

auto Window::get_parent() -> intptr_t { return ::GetWindowLongPtrA(m_hwnd.get(), GWLP_HWNDPARENT); }

auto Window::get_placement() -> ::WINDOWPLACEMENT
{
    ::WINDOWPLACEMENT wp{sizeof(::WINDOWPLACEMENT)};
    ::GetWindowPlacement(m_hwnd.get(), &wp);

    return wp;
}

auto Window::reveal() -> bool { return ::ShowWindow(m_hwnd.get(), SW_NORMAL); }

auto Window::show() -> bool { return ::ShowWindow(m_hwnd.get(), SW_SHOW); }

auto Window::hide() -> bool { return ::ShowWindow(m_hwnd.get(), SW_HIDE); }

auto Window::maximize() -> bool { return ::ShowWindow(m_hwnd.get(), SW_MAXIMIZE); }

auto Window::restore() -> bool { return ::ShowWindow(m_hwnd.get(), SW_RESTORE); }

auto Window::is_visible() -> bool { return ::IsWindowVisible(m_hwnd.get()); }

auto Window::focus() -> ::HWND { return ::SetFocus(m_hwnd.get()); }

auto Window::is_focus() -> bool { return ::GetFocus() == m_hwnd.get(); }

auto Window::foreground() -> bool { return ::SetForegroundWindow(m_hwnd.get()); }

auto Window::is_foreground() -> bool { return ::GetForegroundWindow() == m_hwnd.get(); }

auto Window::active() -> bool { return ::SetActiveWindow(m_hwnd.get()); }

auto Window::top() -> bool { return ::BringWindowToTop(m_hwnd.get()); }

auto Window::toggle_maximize() -> void
{
    auto wp{get_placement()};

    if ((get_style() & WS_OVERLAPPEDWINDOW) && wp.showCmd == 3) { restore(); }

    else { maximize(); }
}

auto Window::toggle_fullscreen() -> void
{
    static RECT pos;

    if (get_style() & WS_OVERLAPPEDWINDOW)
    {
        ::MONITORINFO mi{sizeof(::MONITORINFO)};
        ::GetWindowRect(m_hwnd.get(), &pos);
        if (::GetMonitorInfoA(::MonitorFromWindow(m_hwnd.get(), MONITOR_DEFAULTTONEAREST), &mi))
        {
            ::SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, get_style() & ~WS_OVERLAPPEDWINDOW);
            ::SetWindowPos(m_hwnd.get(), HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                           mi.rcMonitor.right - mi.rcMonitor.left,
                           mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED);
        }
    }

    else
    {
        ::SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, get_style() | WS_OVERLAPPEDWINDOW);
        ::SetWindowPos(m_hwnd.get(), nullptr, pos.left, pos.top, (pos.right - pos.left),
                       (pos.bottom - pos.top), SWP_FRAMECHANGED);
    }
}

auto Window::toggle_topmost() -> void
{
    ::FLASHWINFO fwi{sizeof(::FLASHWINFO)};
    fwi.hwnd = m_hwnd.get();
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    if (get_ex_style() & WS_EX_TOPMOST)
    {
        ::SetWindowPos(m_hwnd.get(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    else { ::SetWindowPos(m_hwnd.get(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); }

    ::FlashWindowEx(&fwi);
}

auto Window::set_title(std::string title) -> bool
{
    return ::SetWindowTextA(m_hwnd.get(), title.c_str());
}

auto Window::set_icon(::HICON icon, bool small, bool big) -> void
{
    if (small)
    {
        ::SendMessageA(m_hwnd.get(), WM_SETICON, ICON_SMALL, reinterpret_cast<::LPARAM>(icon));
    }

    if (big)
    {
        ::SendMessageA(m_hwnd.get(), WM_SETICON, ICON_BIG, reinterpret_cast<::LPARAM>(icon));
    }
}

auto Window::border(bool enabled) -> void
{
    ::SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE,
                        enabled ? (get_style() | WS_BORDER) : (get_style() & ~WS_BORDER));
    ::SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::set_overlapped() -> void
{
    ::SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
    ::SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::set_popup() -> void
{
    ::SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
    ::SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::set_child() -> void
{
    ::SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_CHILD);
    ::SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::reparent(::HWND parent) -> void
{
    if (parent)
    {
        ::SetParent(m_hwnd.get(), parent);
        set_child();
    }

    else
    {
        ::SetParent(m_hwnd.get(), nullptr);
        set_popup();
    }
}

auto Window::position() -> void
{
    ::GetClientRect(m_hwnd.get(), &m_client.rect);

    m_client.x = m_client.rect.left;
    m_client.y = m_client.rect.top;
    m_client.width = m_client.rect.right - m_client.rect.left;
    m_client.height = m_client.rect.bottom - m_client.rect.top;

    ::GetWindowRect(m_hwnd.get(), &m_window.rect);

    m_window.x = m_window.rect.left;
    m_window.y = m_window.rect.top;
    m_window.width = m_window.rect.right - m_window.rect.left;
    m_window.height = m_window.rect.bottom - m_window.rect.top;

    m_dpi = get_dpi();
    m_scale = get_scale();
}

auto Window::resize() -> void { ::SendMessageA(m_hwnd.get(), WM_SIZE, 0, 0); }

auto Window::theme() -> void
{
    if (glow::check_theme()) { dwm_dark_mode(true); }

    else { dwm_dark_mode(false); }
}

auto Window::dwm_dark_mode(bool enabled) -> void
{
    if (enabled)
    {
        auto useImmersiveDarkMode{TRUE};
        ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_USE_IMMERSIVE_DARK_MODE,
                                &useImmersiveDarkMode, sizeof(useImmersiveDarkMode));
    }

    else
    {
        auto useImmersiveDarkMode{FALSE};
        ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_USE_IMMERSIVE_DARK_MODE,
                                &useImmersiveDarkMode, sizeof(useImmersiveDarkMode));
    }
}

auto Window::dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE backdrop) -> void
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

auto Window::dwm_rounded_corners(bool enable) -> void
{
    if (enable)
    {
        auto corner{::DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_ROUND};
        ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_WINDOW_CORNER_PREFERENCE, &corner,
                                sizeof(corner));
    }

    else
    {
        auto corner{::DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DONOTROUND};
        ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_WINDOW_CORNER_PREFERENCE, &corner,
                                sizeof(corner));
    }
}

auto Window::dwm_cloak(bool enable) -> void
{
    if (enable)
    {
        auto cloak{TRUE};
        ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_CLOAK, &cloak, sizeof(cloak));
    }

    else
    {
        auto cloak{FALSE};
        ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_CLOAK, &cloak, sizeof(cloak));
    }
}

auto Window::dwm_caption_color(bool enable) -> void
{
    if (enable)
    {
        auto captionColor{DWMWA_COLOR_DEFAULT};
        ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_CAPTION_COLOR, &captionColor,
                                sizeof(captionColor));
    }

    else
    {
        auto captionColor{DWMWA_COLOR_NONE};
        ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_CAPTION_COLOR, &captionColor,
                                sizeof(captionColor));
    }
}

auto Window::dwm_set_caption_color(::COLORREF color) -> void
{
    ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_CAPTION_COLOR, &color, sizeof(color));
}

auto Window::dwm_border_color(bool enable) -> void
{
    if (enable)
    {
        auto borderColor{DWMWA_COLOR_DEFAULT};
        ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_BORDER_COLOR, &borderColor,
                                sizeof(borderColor));
    }

    else
    {
        auto borderColor{DWMWA_COLOR_NONE};
        ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_BORDER_COLOR, &borderColor,
                                sizeof(borderColor));
    }
}

auto Window::dwm_set_border_color(::COLORREF color) -> void
{
    ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_BORDER_COLOR, &color, sizeof(color));
}

auto Window::dwm_set_text_color(::COLORREF color) -> void
{
    ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_TEXT_COLOR, &color, sizeof(color));
}

auto Window::dwm_reset_text_color() -> void
{
    auto textColor{DWMWA_COLOR_DEFAULT};
    ::DwmSetWindowAttribute(m_hwnd.get(), ::DWMWA_TEXT_COLOR, &textColor, sizeof(textColor));
}

auto CALLBACK Window::WndProc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam)
    -> ::LRESULT
{
    auto self{instance_from_wnd_proc<Window>(hWnd, uMsg, lParam)};

    if (self) { return self->default_wnd_proc(hWnd, uMsg, wParam, lParam); }

    else { return ::DefWindowProcA(hWnd, uMsg, wParam, lParam); }
}

auto Window::default_wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam)
    -> ::LRESULT
{
    switch (uMsg)
    {
        case WM_CLOSE: return on_close(wParam, lParam);
        case WM_CREATE: return on_create(wParam, lParam);
        case WM_DPICHANGED: return on_dpi_changed(wParam, lParam);
    }

    return wnd_proc(hWnd, uMsg, wParam, lParam);
}

auto Window::wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return ::DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::on_close(WPARAM wParam, LPARAM lParam) -> int
{
    m_hwnd.reset();

    return 0;
}

auto Window::on_create(WPARAM wParam, LPARAM lParam) -> int
{
    position();

    return 0;
}

auto Window::on_dpi_changed(WPARAM wParam, LPARAM lParam) -> int
{
    position();

    return 0;
}
} // namespace glow
