// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "gui.hxx"

namespace glow
{
namespace gui
{
GdiPlus::GdiPlus()
    : m_gdiplusStatus{Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, nullptr)}
{
    if (m_gdiplusStatus != Gdiplus::Status::Ok) throw std::runtime_error("GDI+ startup failure");
}

GdiPlus::~GdiPlus()
{
    if (m_gdiplusStatus == Gdiplus::Status::Ok) Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

constexpr auto operator!(Gdiplus::Status gdiplusStatus) noexcept
{
    return static_cast<std::underlying_type_t<Gdiplus::Status>>(gdiplusStatus);
}

CoInitialize::CoInitialize() : m_result{CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)}
{
    glow::console::hresult_check(m_result);
}

CoInitialize::~CoInitialize()
{
    if (SUCCEEDED(m_result)) CoUninitialize();
}

CoInitialize::operator HRESULT() const { return m_result; }

SystemColors::SystemColors() : settings{winrt::Windows::UI::ViewManagement::UISettings()}
{
    update();
}

auto SystemColors::update() -> void
{
    color.accent = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
    color.accentDark1 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1);
    color.accentDark2 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2);
    color.accentDark3 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3);
    color.accentLight1 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1);
    color.accentLight2 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2);
    color.accentLight3 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3);
    color.background =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Background);
    color.foreground =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);

    string.accent = to_string(color.accent);
    string.accentDark1 = to_string(color.accentDark1);
    string.accentDark2 = to_string(color.accentDark2);
    string.accentDark3 = to_string(color.accentDark3);
    string.accentLight1 = to_string(color.accentLight1);
    string.accentLight2 = to_string(color.accentLight2);
    string.accentLight3 = to_string(color.accentLight3);
    string.background = to_string(color.background);
    string.foreground = to_string(color.foreground);

    colorref.accent = to_colorref(color.accent);
    colorref.accentDark1 = to_colorref(color.accentDark1);
    colorref.accentDark2 = to_colorref(color.accentDark2);
    colorref.accentDark3 = to_colorref(color.accentDark3);
    colorref.accentLight1 = to_colorref(color.accentLight1);
    colorref.accentLight2 = to_colorref(color.accentLight2);
    colorref.accentLight3 = to_colorref(color.accentLight3);
    colorref.background = to_colorref(color.background);
    colorref.foreground = to_colorref(color.foreground);

    map.emplace("accent", string.accent);
    map.emplace("accentDark1", string.accentDark1);
    map.emplace("accentDark2", string.accentDark2);
    map.emplace("accentDark3", string.accentDark3);
    map.emplace("accentLight1", string.accentLight1);
    map.emplace("accentLight2", string.accentLight2);
    map.emplace("accentLight3", string.accentLight3);
    map.emplace("background", string.background);
    map.emplace("foreground", string.foreground);
}

auto SystemColors::to_string(winrt::Windows::UI::Color uiColor) -> std::string
{
    return std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", uiColor.R, uiColor.G, uiColor.B, uiColor.A);
}

auto SystemColors::to_colorref(winrt::Windows::UI::Color uiColor) -> COLORREF
{
    return RGB(std::ranges::clamp(static_cast<int>(uiColor.R), 0, 255),
               std::ranges::clamp(static_cast<int>(uiColor.G), 0, 255),
               std::ranges::clamp(static_cast<int>(uiColor.B), 0, 255));
}

Position::Position()
    : x{0}, y{0}, width{0}, height{0}, maximize{false}, fullscreen{false}, topmost{false}, dpi{0},
      scale{0.0f}
{}

Notification::Notification() : nmhdr{} {}

auto message_loop() -> int
{
    MSG msg{};
    int r{};

    while ((r = GetMessageA(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) { glow::console::hresult_check(glow::console::last_error()); }

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
    glow::console::hresult_check(GetAvailableCoreWebView2BrowserVersionString(nullptr, &buffer));

    return glow::text::narrow(buffer.get());
}

auto get_class_info(ATOM& atom, WNDCLASSEXA& wndClass) -> bool
{
    if (GetClassInfoExA(GetModuleHandleA(nullptr), MAKEINTATOM(atom), &wndClass)) return true;

    else return false;
}

auto rect_to_position(const RECT& rect) -> Position
{
    Position position;

    position.x = rect.left;
    position.y = rect.top;
    position.width = rect.right - rect.left;
    position.height = rect.bottom - rect.top;

    return position;
}

auto position_to_rect(const Position& position) -> RECT
{
    RECT rect{};

    rect.left = position.x;
    rect.top = position.y;
    rect.right = position.width;
    rect.bottom = position.height;

    return rect;
}

auto clamp_color(int value) -> int { return std::ranges::clamp(value, 0, 255); }

auto make_colorref(int r, int g, int b) -> COLORREF
{
    return RGB(clamp_color(r), clamp_color(g), clamp_color(b));
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
            auto SetPreferredAppMode{reinterpret_cast<fnSetPreferredAppMode>(ord135)};
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

        auto AllowDarkModeForWindow{reinterpret_cast<fnAllowDarkModeForWindow>(ord133)};
        auto FlushMenuThemes{reinterpret_cast<fnFlushMenuThemes>(ord136)};

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
} // namespace gui
} // namespace glow
