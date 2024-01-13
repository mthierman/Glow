// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/gui.hxx>

namespace glow::gui
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

constexpr auto operator+(Gdiplus::Status gdiplusStatus) noexcept
{
    return static_cast<std::underlying_type_t<Gdiplus::Status>>(gdiplusStatus);
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

// void to_json(nlohmann::json& j, const WindowPosition& windowPosition)
// {
//     j = nlohmann::json{
//         {"x", windowPosition.x},
//         {"y", windowPosition.y},
//         {"width", windowPosition.width},
//         {"height", windowPosition.height},
//         {"maximized", windowPosition.maximized},
//         {"fullscreen", windowPosition.fullscreen},
//         {"topmost", windowPosition.topmost},
//         {"dpi", windowPosition.dpi},
//         {"scale", windowPosition.scale},
//     };
// }

// void from_json(const nlohmann::json& j, WindowPosition& windowPosition)
// {
//     j.at("x").get_to(windowPosition.x);
//     j.at("y").get_to(windowPosition.y);
//     j.at("width").get_to(windowPosition.width);
//     j.at("height").get_to(windowPosition.height);
//     j.at("maximized").get_to(windowPosition.maximized);
//     j.at("fullscreen").get_to(windowPosition.fullscreen);
//     j.at("topmost").get_to(windowPosition.topmost);
//     j.at("dpi").get_to(windowPosition.dpi);
//     j.at("scale").get_to(windowPosition.scale);
// }

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

// auto rect_to_position(const RECT& rect) -> WindowPosition
// {
//     WindowPosition windowPosition;

//     windowPosition.x = rect.left;
//     windowPosition.y = rect.top;
//     windowPosition.width = rect.right - rect.left;
//     windowPosition.height = rect.bottom - rect.top;

//     return windowPosition;
// }

// auto position_to_rect(const WindowPosition& windowPosition) -> RECT
// {
//     RECT rect{};

//     rect.left = windowPosition.x;
//     rect.top = windowPosition.y;
//     rect.right = windowPosition.width;
//     rect.bottom = windowPosition.height;

//     return rect;
// }

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
} // namespace glow::gui
