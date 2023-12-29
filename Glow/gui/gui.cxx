// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <gui/gui.hxx>

namespace glow::gui
{

auto message_loop() -> void
{
    MSG msg{};
    int r{};

    while ((r = GetMessageA(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) return;

        else
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }
}

auto is_dark() -> bool
{
    auto settings{winrt::Windows::UI::ViewManagement::UISettings()};
    auto fg{settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};

    if (((5 * fg.G) + (2 * fg.R) + fg.B) > (8 * 128)) return true;

    return false;
}

auto set_theme(HWND hwnd) -> void
{
    if (is_dark())
    {
        auto useImmersiveDarkMode{TRUE};
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
                              sizeof(useImmersiveDarkMode));
    }

    else
    {
        auto useImmersiveDarkMode{FALSE};
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
                              sizeof(useImmersiveDarkMode));
    }
}

auto set_window_cloak(HWND hwnd, bool enabled) -> void
{
    if (enabled)
    {
        auto cloak{TRUE};
        DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloak, sizeof(cloak));
    }

    else
    {
        auto cloak{FALSE};
        DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloak, sizeof(cloak));
    }
}

auto set_caption_color(HWND hwnd, bool enabled) -> void
{
    if (enabled)
    {
        auto captionColor{DWMWA_COLOR_DEFAULT};
        DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));
    }

    else
    {
        auto captionColor{DWMWA_COLOR_NONE};
        DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));
    }
}

auto set_border_color(HWND hwnd, bool enabled) -> void
{
    if (enabled)
    {
        auto borderColor{DWMWA_COLOR_DEFAULT};
        DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
    }

    else
    {
        auto borderColor{DWMWA_COLOR_NONE};
        DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
    }
}

auto set_system_backdrop(HWND hwnd, DWM_SYSTEMBACKDROP_TYPE backdropType) -> void
{
    // MARGINS m{-1};
    MARGINS m{0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
    if (FAILED(DwmExtendFrameIntoClientArea(hwnd, &m))) return;

    if (FAILED(DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType,
                                     sizeof(&backdropType))))
        return;
}

auto set_preferred_app_mode() -> void
{
    enum class PreferredAppMode
    {
        Default,
        AllowDark,
        ForceDark,
        ForceLight,
        Max
    };
    using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);

    auto uxtheme{LoadLibraryExA("uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

    if (uxtheme)
    {
        auto ord135{GetProcAddress(uxtheme, MAKEINTRESOURCEA(135))};

        if (ord135)
        {
            auto SetPreferredAppMode{std::bit_cast<fnSetPreferredAppMode>(ord135)};
            SetPreferredAppMode(PreferredAppMode::AllowDark);
        }

        FreeLibrary(uxtheme);
    }
}

auto allow_dark_mode(HWND hwnd, bool enable) -> void
{
    auto uxtheme{LoadLibraryExA("uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

    using fnAllowDarkModeForWindow = bool(WINAPI*)(HWND hWnd, bool allow);

    fnAllowDarkModeForWindow AllowDarkModeForWindow = nullptr;

    AllowDarkModeForWindow =
        reinterpret_cast<fnAllowDarkModeForWindow>(GetProcAddress(uxtheme, MAKEINTRESOURCEA(133)));
    // auto _AllowDarkModeForWindow{std::bit_cast<fnAllowDarkModeForWindow>(ord133)};
    AllowDarkModeForWindow(hwnd, enable);
    FreeLibrary(uxtheme);
}

auto window_maximize(HWND hwnd) -> bool
{
    auto style{GetWindowLongPtrA(hwnd, GWL_STYLE)};

    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(hwnd, &wp);

    if ((style & WS_OVERLAPPEDWINDOW) && wp.showCmd == 3)
    {
        ShowWindow(hwnd, SW_SHOWNORMAL);

        return false;
    }

    else
    {
        ShowWindow(hwnd, SW_MAXIMIZE);

        return true;
    }
}

auto window_fullscreen(HWND hwnd) -> bool
{
    static RECT pos;

    auto style{GetWindowLongPtrA(hwnd, GWL_STYLE)};

    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi{sizeof(mi)};
        GetWindowRect(hwnd, &pos);
        if (GetMonitorInfoA(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi))
        {
            SetWindowLongPtrA(hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED);
        }

        return true;
    }

    else
    {
        SetWindowLongPtrA(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(hwnd, nullptr, pos.left, pos.top, (pos.right - pos.left),
                     (pos.bottom - pos.top), SWP_FRAMECHANGED);

        return false;
    }
}

auto window_topmost(HWND hwnd) -> bool
{
    FLASHWINFO fwi{sizeof(FLASHWINFO)};
    fwi.hwnd = hwnd;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    auto style{GetWindowLongPtrA(hwnd, GWL_EXSTYLE)};

    if (style & WS_EX_TOPMOST)
    {
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);

        return false;
    }

    else
    {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);

        return true;
    }
}

auto show_normal(HWND hwnd) -> void { ShowWindow(hwnd, SW_SHOWNORMAL); }

auto show(HWND hwnd) -> void { ShowWindow(hwnd, SW_SHOW); }

auto hide(HWND hwnd) -> void { ShowWindow(hwnd, SW_HIDE); }

auto set_title(HWND hwnd, std::string title) -> void { SetWindowTextA(hwnd, title.c_str()); }

auto set_border(HWND hwnd, bool enabled) -> void
{
    auto style{GetWindowLongPtrA(hwnd, GWL_STYLE)};

    SetWindowLongPtrA(hwnd, GWL_STYLE, enabled ? (style | WS_BORDER) : (style & ~WS_BORDER));
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto set_child(HWND hwnd) -> void
{
    SetWindowLongPtrA(hwnd, GWL_STYLE, WS_CHILD);
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto set_popup(HWND hwnd) -> void
{
    SetWindowLongPtrA(hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto set_overlapped(HWND hwnd) -> void
{
    SetWindowLongPtrA(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

} // namespace glow::gui
