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
    : m_name{name}, m_id{id}, m_style{style}, m_exStyle{exStyle}, m_x{x}, m_y{y}, m_width{width},
      m_height{height}, m_parent{parent}, m_menu{menu}
{
    register_class();
}

Window::~Window() {}

auto Window::register_class() -> void
{
    if (!::GetClassInfoExA(::GetModuleHandleA(nullptr), m_name.c_str(), &m_wcex))
    {
        m_wcex.lpszClassName = m_name.c_str();
        m_wcex.lpszMenuName = 0;
        m_wcex.lpfnWndProc = StaticWndProc;
        m_wcex.style = 0;
        m_wcex.cbClsExtra = 0;
        m_wcex.cbWndExtra = sizeof(intptr_t);
        m_wcex.hInstance = ::GetModuleHandleA(nullptr);
        m_wcex.hbrBackground = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
        m_wcex.hCursor = static_cast<HCURSOR>(
            ::LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
        m_wcex.hIcon = m_icon.get() ? m_icon.get() : m_defaultIcon.get();
        m_wcex.hIconSm = m_icon.get() ? m_icon.get() : m_defaultIcon.get();

        if (::RegisterClassExA(&m_wcex) == 0)
        {
            throw std::runtime_error("Window class registration failure");
        }
    }
}

auto Window::create_window() -> void
{
    if (::CreateWindowExA(m_exStyle, m_wcex.lpszClassName, m_wcex.lpszClassName, m_style, m_x, m_y,
                          m_width, m_height, m_parent, m_menu, ::GetModuleHandleA(nullptr),
                          this) == nullptr)
    {
        throw std::runtime_error("Window creation failure");
    }
}

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

auto Window::get_parent() -> ::HWND
{
    return reinterpret_cast<::HWND>(::GetWindowLongPtrA(m_hwnd.get(), GWLP_HWNDPARENT));
}

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

auto Window::close() -> void { ::SendMessageA(m_hwnd.get(), WM_CLOSE, 0, 0); }

auto Window::is_visible() -> bool { return ::IsWindowVisible(m_hwnd.get()); }

auto Window::focus() -> ::HWND { return ::SetFocus(m_hwnd.get()); }

auto Window::is_focus() -> bool { return ::GetFocus() == m_hwnd.get(); }

auto Window::foreground() -> bool { return ::SetForegroundWindow(m_hwnd.get()); }

auto Window::is_foreground() -> bool { return ::GetForegroundWindow() == m_hwnd.get(); }

auto Window::active() -> bool { return ::SetActiveWindow(m_hwnd.get()); }

auto Window::top() -> bool { return ::BringWindowToTop(m_hwnd.get()); }

auto Window::topmost(bool topmost) -> bool
{
    return ::SetWindowPos(m_hwnd.get(), topmost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0,
                          SWP_NOMOVE | SWP_NOSIZE);
}

auto Window::is_topmost() -> bool { return get_ex_style() & WS_EX_TOPMOST; }

auto Window::flash() -> void
{
    ::FLASHWINFO fwi{sizeof(::FLASHWINFO)};
    fwi.hwnd = m_hwnd.get();
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    ::FlashWindowEx(&fwi);
}

auto Window::toggle_maximize() -> void
{
    auto wp{get_placement()};

    if ((is_overlapped()) && wp.showCmd == 3) { restore(); }

    else { maximize(); }
}

auto Window::toggle_fullscreen() -> void
{
    static RECT pos;

    if (is_overlapped())
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
    if (is_topmost()) { topmost(false); }

    else { topmost(true); }

    flash();
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

auto Window::is_overlapped() -> bool { return get_style() & WS_OVERLAPPEDWINDOW; }

auto Window::set_popup() -> void
{
    ::SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
    ::SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::is_popup() -> bool { return get_style() & WS_POPUP; }

auto Window::set_child() -> void
{
    ::SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_CHILD);
    ::SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::is_child() -> bool { return get_style() & WS_CHILD; }

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

auto CALLBACK Window::StaticWndProc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam)
    -> ::LRESULT
{
    auto self{instance_from_wnd_proc<Window>(hWnd, uMsg, lParam)};

    if (self)
    {
        switch (uMsg)
        {
            case WM_CLOSE: return self->on_close(wParam, lParam);
            case WM_CREATE: return self->on_create(wParam, lParam);
            case WM_DPICHANGED: return self->on_dpi_changed(wParam, lParam);
        }

        return self->WndProc(uMsg, wParam, lParam);
    }

    else { return ::DefWindowProcA(hWnd, uMsg, wParam, lParam); }
}

auto Window::WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    return ::DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto Window::on_close(::WPARAM wParam, ::LPARAM lParam) -> int
{
    m_hwnd.reset();

    return 0;
}

auto Window::on_create(::WPARAM wParam, ::LPARAM lParam) -> int
{
    position();

    return 0;
}

auto Window::on_dpi_changed(::WPARAM wParam, ::LPARAM lParam) -> int
{
    position();

    return 0;
}
} // namespace glow
