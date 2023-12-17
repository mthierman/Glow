// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "gui.hxx"

namespace glow::gui
{

// https://stackoverflow.com/questions/117792/best-method-for-storing-this-pointer-for-use-in-wndproc
// auto register_window() -> ATOM
// {
//     HCURSOR cursor{static_cast<HCURSOR>(
//         LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

//     HICON icon{static_cast<HICON>(
//         LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

//     HBRUSH background{static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))};

//     WNDCLASSEX wcex{sizeof(WNDCLASSEX)};
//     wcex.lpszClassName = "TEST";
//     wcex.lpszMenuName = "TEST";
//     wcex.lpfnWndProc = DefWindowProcA;
//     wcex.style = 0;
//     wcex.cbClsExtra = 0;
//     wcex.cbWndExtra = sizeof(void*);
//     wcex.hInstance = GetModuleHandleA(nullptr);
//     wcex.hbrBackground = background;
//     wcex.hCursor = cursor;
//     wcex.hIcon = icon;
//     wcex.hIconSm = icon;

//     return RegisterClassExA(&wcex);
// }

// auto create_window(ATOM atom) -> HWND
// {
//     return CreateWindowExA(0, MAKEINTATOM(atom), "TEST", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
//                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
//                            nullptr, GetModuleHandleA(nullptr), nullptr);
// }

// auto show_window(HWND hwnd) -> void { ShowWindow(hwnd, SW_SHOWDEFAULT); }

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

    return;
}

auto check_theme() -> bool
{
    auto settings{winrt::Windows::UI::ViewManagement::UISettings()};
    auto fg{settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};

    if (((5 * fg.G) + (2 * fg.R) + fg.B) > (8 * 128)) return true;

    return false;
}

auto set_darkmode(HWND hwnd) -> bool
{
    auto dark{TRUE};
    auto light{FALSE};

    if (check_theme())
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

        return true;
    }

    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &light, sizeof(light));

    return false;
}

auto set_darktitle() -> bool
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

    if (!uxtheme) return false;

    auto ord135{GetProcAddress(uxtheme, MAKEINTRESOURCEA(135))};

    if (!ord135) return false;

    auto SetPreferredAppMode{std::bit_cast<fnSetPreferredAppMode>(ord135)};
    SetPreferredAppMode(PreferredAppMode::AllowDark);
    FreeLibrary(uxtheme);

    return true;
}

auto window_cloak(HWND hwnd) -> bool
{
    auto cloak{TRUE};

    if (FAILED(DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloak, sizeof(cloak)))) return false;

    return true;
}

auto window_uncloak(HWND hwnd) -> bool
{
    auto uncloak{FALSE};

    if (FAILED(DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &uncloak, sizeof(uncloak)))) return false;

    return true;
}

auto window_mica(HWND hwnd) -> bool
{
    MARGINS m{0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
    auto backdrop{DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW};

    if (FAILED(DwmExtendFrameIntoClientArea(hwnd, &m))) return false;

    if (FAILED(
            DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop))))
        return false;

    return true;
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

} // namespace glow::gui
