// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/glow.hxx>

namespace glow::gui
{
SystemColors::SystemColors()
{
    accent = color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
    accentDark1 = color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1);
    accentDark2 = color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2);
    accentDark3 = color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3);
    accentLight1 = color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1);
    accentLight2 = color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2);
    accentLight3 = color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3);
    background = color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::Background);
    foreground = color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
};

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

void to_json(nlohmann::json& j, const WindowPosition& windowPosition)
{
    j = nlohmann::json{
        {"x", windowPosition.x},
        {"y", windowPosition.y},
        {"width", windowPosition.width},
        {"height", windowPosition.height},
        {"maximized", windowPosition.maximized},
        {"fullscreen", windowPosition.fullscreen},
        {"topmost", windowPosition.topmost},
        {"dpi", windowPosition.dpi},
        {"scale", windowPosition.scale},
    };
}

void from_json(const nlohmann::json& j, WindowPosition& windowPosition)
{
    j.at("x").get_to(windowPosition.x);
    j.at("y").get_to(windowPosition.y);
    j.at("width").get_to(windowPosition.width);
    j.at("height").get_to(windowPosition.height);
    j.at("maximized").get_to(windowPosition.maximized);
    j.at("fullscreen").get_to(windowPosition.fullscreen);
    j.at("topmost").get_to(windowPosition.topmost);
    j.at("dpi").get_to(windowPosition.dpi);
    j.at("scale").get_to(windowPosition.scale);
}

void to_json(nlohmann::json& j, const SystemColors& systemColors)
{
    j = nlohmann::json{
        {"accent", systemColors.accent},
        {"accentDark1", systemColors.accentDark1},
        {"accentDark2", systemColors.accentDark2},
        {"accentDark3", systemColors.accentDark3},
        {"accentLight1", systemColors.accentLight1},
        {"accentLight2", systemColors.accentLight2},
        {"accentLight3", systemColors.accentLight3},
        {"background", systemColors.background},
        {"foreground", systemColors.foreground},
    };
}

void from_json(const nlohmann::json& j, SystemColors& systemColors)
{
    j.at("accent").get_to(systemColors.accent);
    j.at("accentDark1").get_to(systemColors.accentDark1);
    j.at("accentDark2").get_to(systemColors.accentDark2);
    j.at("accentDark3").get_to(systemColors.accentDark3);
    j.at("accentLight1").get_to(systemColors.accentLight1);
    j.at("accentLight2").get_to(systemColors.accentLight2);
    j.at("accentLight3").get_to(systemColors.accentLight3);
    j.at("background").get_to(systemColors.background);
    j.at("foreground").get_to(systemColors.foreground);
}

auto message_loop() -> int
{
    MSG msg{};
    int r{};

    while ((r = GetMessageA(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) { glow::console::hresult_check(HRESULT_FROM_WIN32(GetLastError())); }

        else
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }

    return 0;
}

auto webview_version() -> std::string
{
    wil::unique_cotaskmem_string buffer;
    if (FAILED(GetAvailableCoreWebView2BrowserVersionString(nullptr, &buffer)))
        throw std::runtime_error("GetAvailableCoreWebView2BrowserVersionString failure");

    return glow::text::narrow(buffer.get());
}

auto get_class_info(ATOM& atom, WNDCLASSEXA& wndClass) -> bool
{
    if (GetClassInfoExA(GetModuleHandleA(nullptr), MAKEINTATOM(atom), &wndClass)) return true;

    else return false;
}

auto rect_to_position(const RECT& rect) -> WindowPosition
{
    WindowPosition windowPosition;

    windowPosition.x = rect.left;
    windowPosition.y = rect.top;
    windowPosition.width = rect.right - rect.left;
    windowPosition.height = rect.bottom - rect.top;

    return windowPosition;
}

auto position_to_rect(const WindowPosition& windowPosition) -> RECT
{
    RECT rect{};

    rect.left = windowPosition.x;
    rect.top = windowPosition.y;
    rect.right = windowPosition.width;
    rect.bottom = windowPosition.height;

    return rect;
}

auto clamp_color(int value) -> int { return std::ranges::clamp(value, 0, 255); }

auto make_colorref(int r, int g, int b) -> COLORREF
{
    return RGB(clamp_color(r), clamp_color(g), clamp_color(b));
}

auto color_to_string(winrt::Windows::UI::ViewManagement::UIColorType colorType) -> std::string
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

auto allow_dark_mode(HWND hWnd, bool enable) -> void
{
    using fnAllowDarkModeForWindow = bool(WINAPI*)(HWND hWnd, bool allow);
    using fnFlushMenuThemes = void(WINAPI*)();
    auto uxtheme{LoadLibraryExA("uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

    if (uxtheme)
    {
        // fnAllowDarkModeForWindow AllowDarkModeForWindow;
        // fnFlushMenuThemes FlushMenuThemes;

        auto ord133{GetProcAddress(uxtheme, MAKEINTRESOURCEA(133))};
        auto ord136{GetProcAddress(uxtheme, MAKEINTRESOURCEA(136))};

        auto AllowDarkModeForWindow{std::bit_cast<fnAllowDarkModeForWindow>(ord133)};
        auto FlushMenuThemes{std::bit_cast<fnFlushMenuThemes>(ord136)};

        AllowDarkModeForWindow(hWnd, enable);
        FlushMenuThemes();
    }

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

// Window::Window(std::string className, bool show) : m_className{className}, m_show{show} {}

// Window::~Window() {}

// auto Window::register_window() -> void
// {
//     if (!GetClassInfoExA(GetModuleHandleA(nullptr), m_className.c_str(), &m_wcex))
//     {
//         m_wcex.lpszClassName = m_className.c_str();
//         m_wcex.lpszMenuName = 0;
//         m_wcex.lpfnWndProc = Window::WndProc;
//         m_wcex.style = 0;
//         m_wcex.cbClsExtra = 0;
//         m_wcex.cbWndExtra = sizeof(void*);
//         m_wcex.hInstance = GetModuleHandleA(nullptr);
//         m_wcex.hbrBackground = m_hbrBackgroundBlack.get();
//         m_wcex.hCursor = m_hCursor.get();
//         m_wcex.hIcon = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();
//         m_wcex.hIconSm = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();

//         RegisterClassExA(&m_wcex);
//     }
// }

// auto Window::create_window() -> void
// {
//     CreateWindowExA(0, m_wcex.lpszClassName, m_wcex.lpszClassName,
//                     WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT,
//                     CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, GetModuleHandleA(nullptr),
//                     this);
// }

// auto Window::operator()(bool show) -> void
// {
//     m_show = show;
//     register_window();
//     create_window();
//     if (m_show) show_normal();
//     m_dpi = dpi();
//     m_scale = scale();
// }

// auto CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
// {
//     auto self{instance_from_wnd_proc<Window>(hWnd, uMsg, lParam)};

//     if (self) return self->handle_wnd_proc(hWnd, uMsg, wParam, lParam);

//     else return DefWindowProcA(hWnd, uMsg, wParam, lParam);
// }

// auto Window::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
// {
//     switch (uMsg)
//     {
//     case WM_CLOSE: return on_close(hWnd, wParam, lParam);
//     }

//     return DefWindowProcA(hWnd, uMsg, wParam, lParam);
// }

// auto Window::on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
// {
//     m_hwnd.reset();

//     return 0;
// }

// auto Window::dpi() -> int64_t { return GetDpiForWindow(m_hwnd.get()); }

// auto Window::scale() -> float
// {
//     return static_cast<float>(dpi()) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
// }

// auto Window::show_normal() -> void { ShowWindow(m_hwnd.get(), SW_SHOWNORMAL); }

// auto Window::show() -> void { ShowWindow(m_hwnd.get(), SW_SHOW); }

// auto Window::hide() -> void { ShowWindow(m_hwnd.get(), SW_HIDE); }

// auto Window::maximize() -> bool
// {
//     auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_STYLE)};

//     WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
//     GetWindowPlacement(m_hwnd.get(), &wp);

//     if ((style & WS_OVERLAPPEDWINDOW) && wp.showCmd == 3)
//     {
//         ShowWindow(m_hwnd.get(), SW_SHOWNORMAL);

//         return false;
//     }

//     else
//     {
//         ShowWindow(m_hwnd.get(), SW_MAXIMIZE);

//         return true;
//     }
// }

// auto Window::fullscreen() -> bool
// {
//     static RECT pos;

//     auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_STYLE)};

//     if (style & WS_OVERLAPPEDWINDOW)
//     {
//         MONITORINFO mi{sizeof(mi)};
//         GetWindowRect(m_hwnd.get(), &pos);
//         if (GetMonitorInfoA(MonitorFromWindow(m_hwnd.get(), MONITOR_DEFAULTTONEAREST), &mi))
//         {
//             SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
//             SetWindowPos(m_hwnd.get(), HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
//                          mi.rcMonitor.right - mi.rcMonitor.left,
//                          mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED);
//         }

//         return true;
//     }

//     else
//     {
//         SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
//         SetWindowPos(m_hwnd.get(), nullptr, pos.left, pos.top, (pos.right - pos.left),
//                      (pos.bottom - pos.top), SWP_FRAMECHANGED);

//         return false;
//     }
// }

// auto Window::topmost() -> bool
// {
//     FLASHWINFO fwi{sizeof(FLASHWINFO)};
//     fwi.hwnd = m_hwnd.get();
//     fwi.dwFlags = FLASHW_CAPTION;
//     fwi.uCount = 1;
//     fwi.dwTimeout = 100;

//     auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_EXSTYLE)};

//     if (style & WS_EX_TOPMOST)
//     {
//         SetWindowPos(m_hwnd.get(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
//         FlashWindowEx(&fwi);

//         return false;
//     }

//     else
//     {
//         SetWindowPos(m_hwnd.get(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
//         FlashWindowEx(&fwi);

//         return true;
//     }
// }

// auto Window::title(std::string title) -> void { SetWindowTextA(m_hwnd.get(), title.c_str()); }

// auto Window::icon(HICON hIcon) -> void
// {
//     SendMessageA(m_hwnd.get(), WM_SETICON, ICON_SMALL, std::bit_cast<LPARAM>(hIcon));
//     SendMessageA(m_hwnd.get(), WM_SETICON, ICON_BIG, std::bit_cast<LPARAM>(hIcon));
// }

// auto Window::border(bool enabled) -> void
// {
//     auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_STYLE)};

//     SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE,
//                       enabled ? (style | WS_BORDER) : (style & ~WS_BORDER));
//     SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
//                  SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
// }

// auto Window::overlapped() -> void
// {
//     SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
//     SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
//                  SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
// }

// auto Window::popup() -> void
// {
//     SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
//     SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
//                  SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
// }

// auto Window::child() -> void
// {
//     SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_CHILD);
//     SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
//                  SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
// }

// auto Window::reparent(HWND parent) -> void
// {
//     if (parent)
//     {
//         SetParent(m_hwnd.get(), parent);
//         child();
//     }

//     else
//     {
//         SetParent(m_hwnd.get(), nullptr);
//         popup();
//     }
// }

// auto Window::client_rect() -> RECT
// {
//     RECT rect{};
//     GetClientRect(m_hwnd.get(), &rect);

//     return rect;
// }

// auto Window::window_rect() -> RECT
// {
//     RECT rect{};
//     GetWindowRect(m_hwnd.get(), &rect);

//     return rect;
// }

// auto Window::client_position() -> Position
// {
//     auto rect{client_rect()};

//     Position p;

//     p.x = rect.left;
//     p.y = rect.top;
//     p.width = rect.right - rect.left;
//     p.height = rect.bottom - rect.top;

//     return p;
// }

// auto Window::window_position() -> Position
// {
//     auto rect{window_rect()};

//     Position p;

//     p.x = rect.left;
//     p.y = rect.top;
//     p.width = rect.right - rect.left;
//     p.height = rect.bottom - rect.top;

//     return p;
// }

// auto Window::dwm_dark_mode(bool enabled) -> void
// {
//     if (enabled)
//     {
//         auto useImmersiveDarkMode{TRUE};
//         DwmSetWindowAttribute(m_hwnd.get(), DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
//                               sizeof(useImmersiveDarkMode));
//     }

//     else
//     {
//         auto useImmersiveDarkMode{FALSE};
//         DwmSetWindowAttribute(m_hwnd.get(), DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
//                               sizeof(useImmersiveDarkMode));
//     }
// }

// auto Window::dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE backdrop) -> void
// {
//     // MARGINS m{-1};
//     MARGINS m{0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
//     if (FAILED(DwmExtendFrameIntoClientArea(m_hwnd.get(), &m))) return;

//     if (FAILED(DwmSetWindowAttribute(m_hwnd.get(), DWMWA_SYSTEMBACKDROP_TYPE, &backdrop,
//                                      sizeof(&backdrop))))
//         return;
// }

// auto Window::dwm_rounded_corners(bool enable) -> void
// {
//     if (enable)
//     {
//         auto corner{DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_ROUND};
//         DwmSetWindowAttribute(m_hwnd.get(), DWMWA_WINDOW_CORNER_PREFERENCE, &corner,
//                               sizeof(corner));
//     }

//     else
//     {
//         auto corner{DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DONOTROUND};
//         DwmSetWindowAttribute(m_hwnd.get(), DWMWA_WINDOW_CORNER_PREFERENCE, &corner,
//                               sizeof(corner));
//     }
// }

// auto Window::dwm_cloak(bool enable) -> void
// {
//     if (enable)
//     {
//         auto cloak{TRUE};
//         DwmSetWindowAttribute(m_hwnd.get(), DWMWA_CLOAK, &cloak, sizeof(cloak));
//     }

//     else
//     {
//         auto cloak{FALSE};
//         DwmSetWindowAttribute(m_hwnd.get(), DWMWA_CLOAK, &cloak, sizeof(cloak));
//     }
// }

// auto Window::dwm_caption_color(bool enable) -> void
// {
//     if (enable)
//     {
//         auto captionColor{DWMWA_COLOR_DEFAULT};
//         DwmSetWindowAttribute(m_hwnd.get(), DWMWA_CAPTION_COLOR, &captionColor,
//                               sizeof(captionColor));
//     }

//     else
//     {
//         auto captionColor{DWMWA_COLOR_NONE};
//         DwmSetWindowAttribute(m_hwnd.get(), DWMWA_CAPTION_COLOR, &captionColor,
//                               sizeof(captionColor));
//     }
// }

// auto Window::dwm_set_caption_color(COLORREF color) -> void
// {
//     DwmSetWindowAttribute(m_hwnd.get(), DWMWA_CAPTION_COLOR, &color, sizeof(color));
// }

// auto Window::dwm_border_color(bool enable) -> void
// {
//     if (enable)
//     {
//         auto borderColor{DWMWA_COLOR_DEFAULT};
//         DwmSetWindowAttribute(m_hwnd.get(), DWMWA_BORDER_COLOR, &borderColor,
//         sizeof(borderColor));
//     }

//     else
//     {
//         auto borderColor{DWMWA_COLOR_NONE};
//         DwmSetWindowAttribute(m_hwnd.get(), DWMWA_BORDER_COLOR, &borderColor,
//         sizeof(borderColor));
//     }
// }

// auto Window::dwm_set_border_color(COLORREF color) -> void
// {
//     DwmSetWindowAttribute(m_hwnd.get(), DWMWA_BORDER_COLOR, &color, sizeof(color));
// }

// auto Window::dwm_set_text_color(COLORREF color) -> void
// {
//     DwmSetWindowAttribute(m_hwnd.get(), DWMWA_TEXT_COLOR, &color, sizeof(color));
// }

// auto Window::dwm_reset_text_color() -> void
// {
//     auto textColor{DWMWA_COLOR_DEFAULT};
//     DwmSetWindowAttribute(m_hwnd.get(), DWMWA_TEXT_COLOR, &textColor, sizeof(textColor));
// }

// MainWindow::MainWindow(std::string className, bool show) : Window{className, show} {}

// auto MainWindow::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
// {
//     switch (uMsg)
//     {
//     case WM_CLOSE: return on_close(hWnd, wParam, lParam);
//     case WM_DESTROY: return on_destroy(hWnd, wParam, lParam);
//     }

//     return DefWindowProcA(hWnd, uMsg, wParam, lParam);
// }

// auto MainWindow::on_destroy(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
// {
//     PostQuitMessage(0);

//     return 0;
// }

// WebView::WebView(int64_t id, HWND parent, std::string url, std::string className, bool show)
//     : Window{className, show}
// {
//     m_id = id;
//     m_parent = parent;
//     m_url = url;
// }

// auto WebView::operator()(bool show) -> void { Window::operator()(show); }

// auto WebView::register_window() -> void
// {
//     if (!GetClassInfoExA(GetModuleHandleA(nullptr), m_className.c_str(), &m_wcex))
//     {
//         m_wcex.lpszClassName = m_className.c_str();
//         m_wcex.lpszMenuName = 0;
//         m_wcex.lpfnWndProc = WebView::WndProc;
//         m_wcex.style = 0;
//         m_wcex.cbClsExtra = 0;
//         m_wcex.cbWndExtra = sizeof(void*);
//         m_wcex.hInstance = GetModuleHandleA(nullptr);
//         m_wcex.hbrBackground = m_hbrBackgroundBlack.get();
//         m_wcex.hCursor = m_hCursor.get();
//         m_wcex.hIcon = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();
//         m_wcex.hIconSm = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();

//         RegisterClassExA(&m_wcex);
//     }
// }

// auto WebView::create_window() -> void
// {
//     CreateWindowExA(0, m_wcex.lpszClassName, m_wcex.lpszClassName, WS_CHILD, 0, 0, 0, 0,
//     m_parent,
//                     std::bit_cast<HMENU>(m_id), GetModuleHandleA(nullptr), this);
// }

// auto WebView::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
// {
//     switch (uMsg)
//     {
//     case WM_CLOSE: return on_close(hWnd, wParam, lParam);
//     case WM_SIZE: return on_size(hWnd, wParam, lParam);
//     }

//     return DefWindowProcA(hWnd, uMsg, wParam, lParam);
// }

// auto WebView::on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
// {
//     if (m_webView.m_controller4)
//     {
//         RECT rect{};
//         GetClientRect(m_hwnd.get(), &rect);
//         m_webView.m_controller4->put_Bounds(rect);
//     }

//     return 0;
// }

// auto WebView::create_webview() -> HRESULT
// {
//     //
//     https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/user-data-folder?tabs=win32
//     //
//     https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/webview2-idl?view=webview2-1.0.2210.55#createcorewebview2environmentwithoptions

//     return CreateCoreWebView2EnvironmentWithOptions(
//         nullptr, nullptr, nullptr,
//         Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
//             [=, this](HRESULT errorCode, ICoreWebView2Environment* createdEnvironment) -> HRESULT
//             {
//                 if (createdEnvironment)
//                 {
//                     glow::console::hresult_check(create_controller(createdEnvironment));
//                 }

//                 return errorCode;
//             })
//             .Get());
// }

// auto WebView::configure_settings() -> HRESULT
// {
//     glow::console::hresult_check(m_webView.m_settings8->put_AreBrowserAcceleratorKeysEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_AreDefaultContextMenusEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_AreDefaultScriptDialogsEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_AreDevToolsEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_AreHostObjectsAllowed(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_HiddenPdfToolbarItems(
//         COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE));
//     glow::console::hresult_check(m_webView.m_settings8->put_IsBuiltInErrorPageEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_IsGeneralAutofillEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_IsPasswordAutosaveEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_IsPinchZoomEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_IsReputationCheckingRequired(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_IsScriptEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_IsStatusBarEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_IsSwipeNavigationEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_IsWebMessageEnabled(true));
//     glow::console::hresult_check(m_webView.m_settings8->put_IsZoomControlEnabled(true));

//     return S_OK;
// }

// auto WebView::create_controller(ICoreWebView2Environment* createdEnvironment) -> HRESULT
// {
//     return createdEnvironment->CreateCoreWebView2Controller(
//         m_hwnd.get(),
//         Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
//             [=, this](HRESULT errorCode, ICoreWebView2Controller* createdController) -> HRESULT
//             {
//                 if (createdController)
//                 {
//                     m_webView.m_controller = createdController;
//                     m_webView.m_controller4 =
//                         m_webView.m_controller.try_query<ICoreWebView2Controller4>();

//                     if (!m_webView.m_controller4) return E_POINTER;

//                     COREWEBVIEW2_COLOR bgColor{0, 0, 0, 0};
//                     glow::console::hresult_check(
//                         m_webView.m_controller4->put_DefaultBackgroundColor(bgColor));

//                     SendMessageA(m_parent, WM_SIZE, 0, 0);
//                     SendMessageA(m_hwnd.get(), WM_SIZE, 0, 0);

//                     glow::console::hresult_check(
//                         m_webView.m_controller->get_CoreWebView2(m_webView.m_core.put()));
//                     m_webView.m_core20 = m_webView.m_core.try_query<ICoreWebView2_20>();

//                     if (!m_webView.m_core20) return E_POINTER;

//                     glow::console::hresult_check(
//                         m_webView.m_core20->get_Settings(m_webView.m_settings.put()));

//                     m_webView.m_settings8 =
//                         m_webView.m_settings.try_query<ICoreWebView2Settings8>();

//                     if (!m_webView.m_settings8) return E_POINTER;

//                     glow::console::hresult_check(configure_settings());

//                     glow::console::hresult_check(
//                         m_webView.m_core20->Navigate(text::widen(m_url).c_str()));

//                     glow::console::hresult_check(context_menu_requested());
//                     glow::console::hresult_check(source_changed());
//                     glow::console::hresult_check(navigation_starting());
//                     glow::console::hresult_check(navigation_completed());
//                     glow::console::hresult_check(web_message_received());
//                     glow::console::hresult_check(accelerator_key_pressed());
//                     glow::console::hresult_check(favicon_changed());
//                     glow::console::hresult_check(document_title_changed());
//                     glow::console::hresult_check(zoom_factor_changed());

//                     if (!m_initialized)
//                     {
//                         m_initialized = true;
//                         initialized();
//                     }
//                 }

//                 return errorCode;
//             })
//             .Get());
// }

// auto WebView::context_menu_requested() -> HRESULT
// {
//     EventRegistrationToken token;

//     return m_webView.m_core20->add_ContextMenuRequested(
//         Microsoft::WRL::Callback<ICoreWebView2ContextMenuRequestedEventHandler>(
//             [=, this](ICoreWebView2* sender,
//                       ICoreWebView2ContextMenuRequestedEventArgs* args) -> HRESULT
//             { return context_menu_requested_handler(sender, args); })
//             .Get(),
//         &token);
// }

// auto WebView::source_changed() -> HRESULT
// {
//     EventRegistrationToken token;

//     return m_webView.m_core20->add_SourceChanged(
//         Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>(
//             [=, this](ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args) ->
//             HRESULT { return source_changed_handler(sender, args); }) .Get(),
//         &token);
// }

// auto WebView::navigation_starting() -> HRESULT
// {
//     EventRegistrationToken token;

//     return m_webView.m_core20->add_NavigationStarting(
//         Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
//             [=, this](ICoreWebView2* sender,
//                       ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT
//             { return navigation_starting_handler(sender, args); })
//             .Get(),
//         &token);
// }

// auto WebView::navigation_completed() -> HRESULT
// {
//     EventRegistrationToken token;

//     return m_webView.m_core20->add_NavigationCompleted(
//         Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
//             [=, this](ICoreWebView2* sender,
//                       ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
//             { return navigation_completed_handler(sender, args); })
//             .Get(),
//         &token);
// }

// auto WebView::web_message_received() -> HRESULT
// {
//     EventRegistrationToken token;

//     return m_webView.m_core20->add_WebMessageReceived(
//         Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
//             [=, this](ICoreWebView2* sender,
//                       ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
//             { return web_message_received_handler(sender, args); })
//             .Get(),
//         &token);
// }

// auto WebView::document_title_changed() -> HRESULT
// {
//     EventRegistrationToken token;

//     return m_webView.m_core20->add_DocumentTitleChanged(
//         Microsoft::WRL::Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
//             [=, this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
//             { return document_title_changed_handler(sender, args); })
//             .Get(),
//         &token);
// }

// auto WebView::favicon_changed() -> HRESULT
// {
//     EventRegistrationToken token;

//     return m_webView.m_core20->add_FaviconChanged(
//         Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
//             [=, this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
//             { return favicon_changed_handler(sender, args); })
//             .Get(),
//         &token);
// }

// auto WebView::accelerator_key_pressed() -> HRESULT
// {
//     EventRegistrationToken token;

//     return m_webView.m_controller4->add_AcceleratorKeyPressed(
//         Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
//             [=, this](ICoreWebView2Controller* sender,
//                       ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT
//             { return accelerator_key_pressed_handler(sender, args); })
//             .Get(),
//         &token);
// }

// auto WebView::zoom_factor_changed() -> HRESULT
// {
//     EventRegistrationToken token;

//     return m_webView.m_controller4->add_ZoomFactorChanged(
//         Microsoft::WRL::Callback<ICoreWebView2ZoomFactorChangedEventHandler>(
//             [=, this](ICoreWebView2Controller* sender, IUnknown* args) -> HRESULT
//             { return zoom_factor_changed_handler(sender, args); })
//             .Get(),
//         &token);
// }

// auto WebView::navigate(std::string url) -> HRESULT
// {
//     auto wideUrl{glow::text::widen(url)};
//     if (m_webView.m_core20)
//         return glow::console::hresult_check(m_webView.m_core20->Navigate(wideUrl.c_str()));

//     else return S_OK;
// }

// auto WebView::post_json(const nlohmann::json jsonMessage) -> HRESULT
// {
//     auto wideUrl{glow::text::widen(jsonMessage)};
//     if (m_webView.m_core20)
//         return glow::console::hresult_check(
//             m_webView.m_core20->PostWebMessageAsJson(wideUrl.c_str()));

//     else return S_OK;
// }
} // namespace glow::gui
