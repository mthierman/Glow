// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/window.hxx>

namespace glow::window
{

Window::Window(std::string className) : m_className(className) {}

Window::~Window() {}

auto Window::create() -> void
{
    WNDCLASSEXA wcex{sizeof(WNDCLASSEXA)};

    if (!GetClassInfoExA(GetModuleHandleA(nullptr), m_className.c_str(), &wcex))
    {
        OutputDebugStringA("Registering Window class...");

        wcex.lpszClassName = m_className.c_str();
        wcex.lpszMenuName = 0;
        wcex.lpfnWndProc = Window::WndProc;
        wcex.style = 0;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(void*);
        wcex.hInstance = GetModuleHandleA(nullptr);
        wcex.hbrBackground = m_hbrBackground.get();
        wcex.hCursor = m_hCursor.get();
        wcex.hIcon = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();
        wcex.hIconSm = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();

        RegisterClassExA(&wcex);
    }

    CreateWindowExA(0, wcex.lpszClassName, wcex.lpszClassName,
                    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT,
                    CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, GetModuleHandleA(nullptr),
                    this);
}

auto CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto self{instance_from_wnd_proc<Window>(hWnd, uMsg, lParam)};

    if (self)
    {
        switch (uMsg)
        {
        case WM_CLOSE: return self->on_close(hWnd, wParam, lParam);
        case WM_DESTROY: return self->on_destroy(hWnd, wParam, lParam);
        }

        return self->handle_wnd_proc(hWnd, uMsg, wParam, lParam);
    }

    else return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    m_hwnd.reset();

    return 0;
}

auto Window::on_destroy(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int { return 0; }

auto Window::dpi() -> int { return GetDpiForWindow(m_hwnd.get()); }

auto Window::scale() -> float
{
    return static_cast<float>(dpi()) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
}

auto Window::show_normal() -> void { ShowWindow(m_hwnd.get(), SW_SHOWNORMAL); }

auto Window::show() -> void { ShowWindow(m_hwnd.get(), SW_SHOW); }

auto Window::hide() -> void { ShowWindow(m_hwnd.get(), SW_HIDE); }

auto Window::maximize() -> bool
{
    auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_STYLE)};

    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(m_hwnd.get(), &wp);

    if ((style & WS_OVERLAPPEDWINDOW) && wp.showCmd == 3)
    {
        ShowWindow(m_hwnd.get(), SW_SHOWNORMAL);

        return false;
    }

    else
    {
        ShowWindow(m_hwnd.get(), SW_MAXIMIZE);

        return true;
    }
}

auto Window::fullscreen() -> bool
{
    static RECT pos;

    auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_STYLE)};

    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi{sizeof(mi)};
        GetWindowRect(m_hwnd.get(), &pos);
        if (GetMonitorInfoA(MonitorFromWindow(m_hwnd.get(), MONITOR_DEFAULTTONEAREST), &mi))
        {
            SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(m_hwnd.get(), HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED);
        }

        return true;
    }

    else
    {
        SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(m_hwnd.get(), nullptr, pos.left, pos.top, (pos.right - pos.left),
                     (pos.bottom - pos.top), SWP_FRAMECHANGED);

        return false;
    }
}

auto Window::topmost() -> bool
{
    FLASHWINFO fwi{sizeof(FLASHWINFO)};
    fwi.hwnd = m_hwnd.get();
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_EXSTYLE)};

    if (style & WS_EX_TOPMOST)
    {
        SetWindowPos(m_hwnd.get(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);

        return false;
    }

    else
    {
        SetWindowPos(m_hwnd.get(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);

        return true;
    }
}

auto Window::title(std::string title) -> void { SetWindowTextA(m_hwnd.get(), title.c_str()); }

auto Window::icon(HICON hIcon) -> void
{
    SendMessageA(m_hwnd.get(), WM_SETICON, ICON_SMALL, std::bit_cast<LPARAM>(hIcon));
    SendMessageA(m_hwnd.get(), WM_SETICON, ICON_BIG, std::bit_cast<LPARAM>(hIcon));
}

auto Window::border(bool enabled) -> void
{
    auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_STYLE)};

    SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE,
                      enabled ? (style | WS_BORDER) : (style & ~WS_BORDER));
    SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::overlapped() -> void
{
    SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
    SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::popup() -> void
{
    SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
    SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::child() -> void
{
    SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_CHILD);
    SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::client_rect() -> RECT
{
    RECT rect{};
    GetClientRect(m_hwnd.get(), &rect);

    return rect;
}

auto Window::window_rect() -> RECT
{
    RECT rect{};
    GetWindowRect(m_hwnd.get(), &rect);

    return rect;
}

auto Window::client_position() -> Position
{
    auto rect{client_rect()};

    Position p;

    p.x = rect.left;
    p.y = rect.top;
    p.width = rect.right - rect.left;
    p.height = rect.bottom - rect.top;

    return p;
}

auto Window::window_position() -> Position
{
    auto rect{window_rect()};

    Position p;

    p.x = rect.left;
    p.y = rect.top;
    p.width = rect.right - rect.left;
    p.height = rect.bottom - rect.top;

    return p;
}

auto Window::dwm_dark_mode(bool enabled) -> void
{
    if (enabled)
    {
        auto useImmersiveDarkMode{TRUE};
        DwmSetWindowAttribute(m_hwnd.get(), DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
                              sizeof(useImmersiveDarkMode));
    }

    else
    {
        auto useImmersiveDarkMode{FALSE};
        DwmSetWindowAttribute(m_hwnd.get(), DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
                              sizeof(useImmersiveDarkMode));
    }
}

auto Window::dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE backdrop) -> void
{
    // MARGINS m{-1};
    MARGINS m{0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
    if (FAILED(DwmExtendFrameIntoClientArea(m_hwnd.get(), &m))) return;

    if (FAILED(DwmSetWindowAttribute(m_hwnd.get(), DWMWA_SYSTEMBACKDROP_TYPE, &backdrop,
                                     sizeof(&backdrop))))
        return;
}

auto Window::dwm_rounded_corners(bool enable) -> void
{
    if (enable)
    {
        auto corner{DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_ROUND};
        DwmSetWindowAttribute(m_hwnd.get(), DWMWA_WINDOW_CORNER_PREFERENCE, &corner,
                              sizeof(corner));
    }

    else
    {
        auto corner{DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DONOTROUND};
        DwmSetWindowAttribute(m_hwnd.get(), DWMWA_WINDOW_CORNER_PREFERENCE, &corner,
                              sizeof(corner));
    }
}

auto Window::dwm_cloak(bool enable) -> void
{
    if (enable)
    {
        auto cloak{TRUE};
        DwmSetWindowAttribute(m_hwnd.get(), DWMWA_CLOAK, &cloak, sizeof(cloak));
    }

    else
    {
        auto cloak{FALSE};
        DwmSetWindowAttribute(m_hwnd.get(), DWMWA_CLOAK, &cloak, sizeof(cloak));
    }
}

auto Window::dwm_caption_color(bool enable) -> void
{
    if (enable)
    {
        auto captionColor{DWMWA_COLOR_DEFAULT};
        DwmSetWindowAttribute(m_hwnd.get(), DWMWA_CAPTION_COLOR, &captionColor,
                              sizeof(captionColor));
    }

    else
    {
        auto captionColor{DWMWA_COLOR_NONE};
        DwmSetWindowAttribute(m_hwnd.get(), DWMWA_CAPTION_COLOR, &captionColor,
                              sizeof(captionColor));
    }
}

auto Window::dwm_set_caption_color(COLORREF color) -> void
{
    DwmSetWindowAttribute(m_hwnd.get(), DWMWA_CAPTION_COLOR, &color, sizeof(color));
}

auto Window::dwm_border_color(bool enable) -> void
{
    if (enable)
    {
        auto borderColor{DWMWA_COLOR_DEFAULT};
        DwmSetWindowAttribute(m_hwnd.get(), DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
    }

    else
    {
        auto borderColor{DWMWA_COLOR_NONE};
        DwmSetWindowAttribute(m_hwnd.get(), DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
    }
}

auto Window::dwm_set_border_color(COLORREF color) -> void
{
    DwmSetWindowAttribute(m_hwnd.get(), DWMWA_BORDER_COLOR, &color, sizeof(color));
}

auto Window::dwm_set_text_color(COLORREF color) -> void
{
    DwmSetWindowAttribute(m_hwnd.get(), DWMWA_TEXT_COLOR, &color, sizeof(color));
}

auto Window::dwm_reset_text_color() -> void
{
    auto textColor{DWMWA_COLOR_DEFAULT};
    DwmSetWindowAttribute(m_hwnd.get(), DWMWA_TEXT_COLOR, &textColor, sizeof(textColor));
}

MainWindow::MainWindow(std::string className) : Window(className) {}

auto MainWindow::on_destroy(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    PostQuitMessage(0);

    return 0;
}

auto message_loop() -> int
{
    MSG msg{};
    int r{};

    while ((r = GetMessageA(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) return 1002;

        else
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }

    return 0;
}

auto clamp_color(int value) -> int { return std::ranges::clamp(value, 0, 255); }

auto make_colorref(int r, int g, int b) -> COLORREF
{
    return RGB(clamp_color(r), clamp_color(g), clamp_color(b));
}

std::string format_color(winrt::Windows::UI::ViewManagement::UIColorType colorType)
{
    auto settings{winrt::Windows::UI::ViewManagement::UISettings()};
    auto accent{settings.GetColorValue(colorType)};

    auto formatted{
        std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", accent.R, accent.G, accent.B, accent.A)};

    return formatted;
}

auto check_theme() -> bool
{
    auto settings{winrt::Windows::UI::ViewManagement::UISettings()};
    auto fg{settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};

    if (((5 * fg.G) + (2 * fg.R) + fg.B) > (8 * 128)) return true;

    return false;
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

auto icon_application() -> HICON
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_error() -> HICON
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_ERROR, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_question() -> HICON
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_QUESTION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_warning() -> HICON
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_WARNING, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_information() -> HICON
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_INFORMATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_winlogo() -> HICON
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_WINLOGO, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_shield() -> HICON
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_SHIELD, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

GdiPlus::GdiPlus()
    : m_gdiplusStatus{Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, nullptr)}
{
    if (m_gdiplusStatus != Gdiplus::Status::Ok) throw std::runtime_error("GDI+ startup failure");
}

GdiPlus::~GdiPlus()
{
    if (m_gdiplusStatus == Gdiplus::Status::Ok) Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

constexpr auto operator+(Gdiplus::Status g) noexcept
{
    return static_cast<std::underlying_type_t<Gdiplus::Status>>(g);
}

CoInitialize::CoInitialize() : m_result{CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)}
{
    if (FAILED(m_result)) throw std::runtime_error("CoInitialize failure");
}

CoInitialize::~CoInitialize()
{
    if (SUCCEEDED(m_result)) CoUninitialize();
}

CoInitialize::operator HRESULT() const { return m_result; }

} // namespace glow::window
