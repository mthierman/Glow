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
Window::Window(std::string name, intptr_t id, ::DWORD style, ::DWORD exStyle, int x, int y,
               int width, int height, ::HWND parent, ::HMENU menu)
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

    dpi();
    scale();
}

Window::~Window() {}

auto Window::hwnd() const -> ::HWND { return m_hwnd.get(); }

auto Window::hwnd(HWND hWnd) -> void { m_hwnd.reset(hWnd); }

auto Window::hicon() const -> ::HICON { return m_hicon.get(); }

auto Window::id() const -> intptr_t { return m_id; }

auto Window::close() -> int
{
    m_hwnd.reset();

    return 0;
}

auto Window::notify(::HWND receiver, CODE code, std::string message) -> void
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

auto Window::dpi() -> void { m_dpi = ::GetDpiForWindow(hwnd()); };

auto Window::scale() -> void
{
    m_scale = static_cast<float>(m_dpi) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
};

auto Window::reveal() -> void { ::ShowWindow(hwnd(), SW_SHOWNORMAL); }

auto Window::show() -> void { ::ShowWindow(hwnd(), SW_SHOW); }

auto Window::hide() -> void { ::ShowWindow(hwnd(), SW_HIDE); }

auto Window::is_visible() -> bool { return ::IsWindowVisible(hwnd()); }

auto Window::focus() -> void { ::SetFocus(hwnd()); }

auto Window::is_focus() -> bool { return ::GetFocus() == hwnd(); }

auto Window::foreground() -> void { ::SetForegroundWindow(hwnd()); }

auto Window::is_foreground() -> bool { return ::GetForegroundWindow() == hwnd(); }

auto Window::active() -> void { ::SetActiveWindow(hwnd()); }

auto Window::top() -> void { ::BringWindowToTop(hwnd()); }

auto Window::maximize() -> void
{
    auto style{::GetWindowLongPtrA(hwnd(), GWL_STYLE)};

    ::WINDOWPLACEMENT wp{sizeof(::WINDOWPLACEMENT)};
    ::GetWindowPlacement(hwnd(), &wp);

    if ((style & WS_OVERLAPPEDWINDOW) && wp.showCmd == 3) { ::ShowWindow(hwnd(), SW_RESTORE); }

    else { ::ShowWindow(hwnd(), SW_MAXIMIZE); }
}

auto Window::fullscreen() -> void
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

auto Window::topmost() -> void
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

auto Window::title(std::string title) -> void { ::SetWindowTextA(hwnd(), title.c_str()); }

auto Window::icon(::HICON icon, bool small, bool big) -> void
{
    if (small) { ::SendMessageA(hwnd(), WM_SETICON, ICON_SMALL, reinterpret_cast<::LPARAM>(icon)); }

    if (big) { ::SendMessageA(hwnd(), WM_SETICON, ICON_BIG, reinterpret_cast<::LPARAM>(icon)); }
}

auto Window::border(bool enabled) -> void
{
    auto style{::GetWindowLongPtrA(hwnd(), GWL_STYLE)};

    ::SetWindowLongPtrA(hwnd(), GWL_STYLE, enabled ? (style | WS_BORDER) : (style & ~WS_BORDER));
    ::SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::overlapped() -> void
{
    ::SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
    ::SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::popup() -> void
{
    ::SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
    ::SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::child() -> void
{
    ::SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_CHILD);
    ::SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::reparent(::HWND parent) -> void
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

auto Window::position() -> void
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

auto Window::size() -> void { ::SendMessageA(hwnd(), WM_SIZE, 0, 0); }

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
        ::DwmSetWindowAttribute(hwnd(), ::DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
    }

    else
    {
        auto corner{::DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DONOTROUND};
        ::DwmSetWindowAttribute(hwnd(), ::DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
    }
}

auto Window::dwm_cloak(bool enable) -> void
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

auto Window::dwm_caption_color(bool enable) -> void
{
    if (enable)
    {
        auto captionColor{DWMWA_COLOR_DEFAULT};
        ::DwmSetWindowAttribute(hwnd(), ::DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));
    }

    else
    {
        auto captionColor{DWMWA_COLOR_NONE};
        ::DwmSetWindowAttribute(hwnd(), ::DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));
    }
}

auto Window::dwm_set_caption_color(::COLORREF color) -> void
{
    ::DwmSetWindowAttribute(hwnd(), ::DWMWA_CAPTION_COLOR, &color, sizeof(color));
}

auto Window::dwm_border_color(bool enable) -> void
{
    if (enable)
    {
        auto borderColor{DWMWA_COLOR_DEFAULT};
        ::DwmSetWindowAttribute(hwnd(), ::DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
    }

    else
    {
        auto borderColor{DWMWA_COLOR_NONE};
        ::DwmSetWindowAttribute(hwnd(), ::DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
    }
}

auto Window::dwm_set_border_color(::COLORREF color) -> void
{
    ::DwmSetWindowAttribute(hwnd(), ::DWMWA_BORDER_COLOR, &color, sizeof(color));
}

auto Window::dwm_set_text_color(::COLORREF color) -> void
{
    ::DwmSetWindowAttribute(hwnd(), ::DWMWA_TEXT_COLOR, &color, sizeof(color));
}

auto Window::dwm_reset_text_color() -> void
{
    auto textColor{DWMWA_COLOR_DEFAULT};
    ::DwmSetWindowAttribute(hwnd(), ::DWMWA_TEXT_COLOR, &textColor, sizeof(textColor));
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
    case WM_CLOSE: return close();
    }

    return wnd_proc(hWnd, uMsg, wParam, lParam);
}

auto Window::wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return ::DefWindowProcA(hWnd, uMsg, wParam, lParam);
}
} // namespace glow
