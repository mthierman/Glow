// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <Unknwn.h>
#include <dwmapi.h>
#include <gdiplus.h>
#include <objbase.h>
#include <shellapi.h>
#include <comdef.h>
#include <ShlObj.h>
#include <wrl.h>

#include <wil/com.h>
#include <wil/result.h>
#include <wil/resource.h>
#include <wil/win32_helpers.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <algorithm>
#include <bit>
#include <cstdint>
#include <filesystem>
#include <format>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <print>
#include <random>
#include <source_location>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <sqlite3.h>

namespace glow
{
namespace console
{
auto argv() -> std::vector<std::string>;
auto hresult_to_string(HRESULT errorCode) -> std::string;
auto hresult_check(HRESULT errorCode) -> HRESULT;
auto debug_hresult(HRESULT errorCode,
                   std::source_location location = std::source_location::current()) -> void;
auto print_hresult(HRESULT errorCode,
                   std::source_location location = std::source_location::current()) -> void;
auto debug(std::string message, std::source_location location = std::source_location::current())
    -> void;
auto print(std::string message, std::source_location location = std::source_location::current())
    -> void;
auto box(std::string message, UINT type = MB_OK | MB_ICONINFORMATION) -> int;
auto shell(std::string message, UINT type = MB_OK | MB_ICONINFORMATION) -> int;
auto stock(std::string message, SHSTOCKICONID icon = SIID_INFO) -> void;

struct Console
{
    Console();
    ~Console();
    FILE* pFile{nullptr};
};
} // namespace console

namespace filesystem
{
struct Database
{
    Database();
    ~Database();
    auto open() -> void;
    auto write() -> void;

  private:
    struct sqlite3_deleter
    {
        auto operator()(sqlite3* pDb) noexcept -> void { sqlite3_close(pDb); }
    };
    using sqlite3_ptr = std::unique_ptr<sqlite3, sqlite3_deleter>;
    sqlite3_ptr p_db;
    std::filesystem::path path;
};

auto known_folder(KNOWNFOLDERID folderId = FOLDERID_LocalAppData) -> std::filesystem::path;
auto program_name() -> std::string;
auto portable() -> std::filesystem::path;
auto wportable() -> std::filesystem::path;
} // namespace filesystem

namespace text
{
auto narrow(std::wstring utf16) -> std::string;
auto widen(std::string utf8) -> std::wstring;
auto randomize(std::string string) -> std::string;
auto random_int64() -> int64_t;
auto random_int32() -> int32_t;
auto create_guid() -> GUID;
auto guid_to_string(GUID guid) -> std::string;
} // namespace text

namespace gui
{
struct GdiPlus
{
    GdiPlus();
    ~GdiPlus();

    Gdiplus::GdiplusStartupInput m_gdiplusStartupInput{};
    ULONG_PTR m_gdiplusToken{};
    Gdiplus::Status m_gdiplusStatus{};
};

struct CoInitialize
{
    CoInitialize();
    ~CoInitialize();

    operator HRESULT() const;
    HRESULT m_result{};
};

struct WindowPosition
{
    int x{};
    int y{};
    int width{};
    int height{};
    bool maximized{};
    bool fullscreen{};
    bool topmost{};
    int64_t dpi{};
    float scale{};
};
void to_json(nlohmann::json& j, const WindowPosition& windowPosition);
void from_json(const nlohmann::json& j, WindowPosition& windowPosition);

struct SystemColors
{
    SystemColors();

    std::string accent;
    std::string accentDark1;
    std::string accentDark2;
    std::string accentDark3;
    std::string accentLight1;
    std::string accentLight2;
    std::string accentLight3;
    std::string background;
    std::string foreground;
};
void to_json(nlohmann::json& j, const SystemColors& systemColors);
void from_json(const nlohmann::json& j, SystemColors& systemColors);

auto message_loop() -> int;
auto webview_version() -> std::string;
auto get_class_info(ATOM& atom, WNDCLASSEXA& wndClass) -> bool;
auto rect_to_position(const RECT& rect) -> WindowPosition;
auto position_to_rect(const WindowPosition& windowPosition) -> RECT;
auto clamp_color(int value) -> int;
auto make_colorref(int r, int g, int b) -> COLORREF;
auto color_to_string(winrt::Windows::UI::ViewManagement::UIColorType colorType) -> std::string;
auto check_theme() -> bool;
auto set_preferred_app_mode() -> void;
auto allow_dark_mode(HWND hWnd, bool enable) -> void;
auto icon_application() -> HICON;
auto icon_error() -> HICON;
auto icon_question() -> HICON;
auto icon_warning() -> HICON;
auto icon_information() -> HICON;
auto icon_winlogo() -> HICON;
auto icon_shield() -> HICON;

template <typename T> T* instance_from_wnd_proc(HWND hWnd, UINT uMsg, LPARAM lParam)
{
    T* self{nullptr};

    if (uMsg == WM_NCCREATE)
    {
        auto lpCreateStruct{std::bit_cast<LPCREATESTRUCT>(lParam)};
        self = static_cast<T*>(lpCreateStruct->lpCreateParams);
        self->m_hwnd.reset(hWnd);
        SetWindowLongPtrA(hWnd, 0, std::bit_cast<LONG_PTR>(self));
    }

    else self = std::bit_cast<T*>(GetWindowLongPtrA(hWnd, 0));

    return self;
}

template <typename T> T* instance(HWND hWnd)
{
    T* self{std::bit_cast<T*>(GetWindowLongPtrA(hWnd, 0))};

    return self;
}

template <typename T> struct MessageWindow
{
    MessageWindow(std::string name = "MessageWindow")
    {
        WNDCLASSEXA wcex{sizeof(WNDCLASSEXA)};

        if (!GetClassInfoExA(GetModuleHandleA(nullptr), name.c_str(), &wcex))
        {
            wcex.lpszClassName = name.c_str();
            wcex.lpszMenuName = 0;
            wcex.lpfnWndProc = T::WndProc;
            wcex.style = 0;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = sizeof(void*);
            wcex.hInstance = GetModuleHandleA(nullptr);
            wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
            wcex.hCursor = static_cast<HCURSOR>(
                LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
            wcex.hIcon = static_cast<HICON>(
                LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
            wcex.hIconSm = static_cast<HICON>(
                LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));

            if (RegisterClassExA(&wcex) == 0)
                throw std::runtime_error("Class registration failure");
        }

        if (CreateWindowExA(0, wcex.lpszClassName, wcex.lpszClassName, 0, 0, 0, 0, 0, HWND_MESSAGE,
                            nullptr, GetModuleHandleA(nullptr), this) == nullptr)
            throw std::runtime_error("Window creation failure");
    }

    auto hwnd() const -> HWND { return m_hwnd.get(); }

    auto close() -> int
    {
        m_hwnd.reset();

        return 0;
    }

  private:
    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        T* self{nullptr};

        if (uMsg == WM_NCCREATE)
        {
            auto lpCreateStruct{std::bit_cast<LPCREATESTRUCT>(lParam)};
            self = static_cast<T*>(lpCreateStruct->lpCreateParams);
            self->m_hwnd.reset(hWnd);
            SetWindowLongPtrA(hWnd, 0, std::bit_cast<LONG_PTR>(self));
        }

        else self = std::bit_cast<T*>(GetWindowLongPtrA(hWnd, 0));

        if (self) { return self->handle_wnd_proc(uMsg, wParam, lParam); }

        else { return DefWindowProcA(hWnd, uMsg, wParam, lParam); }
    }

    wil::unique_hwnd m_hwnd;
};

template <typename T> struct BaseWindow
{
    BaseWindow(std::string name = "BaseWindow",
               DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, DWORD dwExStyle = 0,
               int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT,
               int nHeight = CW_USEDEFAULT, HWND hwndParent = nullptr, HMENU hMenu = nullptr)
    {
        WNDCLASSEXA wcex{sizeof(WNDCLASSEXA)};

        if (!GetClassInfoExA(GetModuleHandleA(nullptr), name.c_str(), &wcex))
        {
            wil::unique_hicon appIcon{
                static_cast<HICON>(LoadImageA(GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101),
                                              IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};

            wcex.lpszClassName = name.c_str();
            wcex.lpszMenuName = 0;
            wcex.lpfnWndProc = T::WndProc;
            wcex.style = 0;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = sizeof(void*);
            wcex.hInstance = GetModuleHandleA(nullptr);
            wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
            wcex.hCursor = static_cast<HCURSOR>(
                LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
            wcex.hIcon = m_icon.get()
                             ? m_icon.get()
                             : static_cast<HICON>(LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON,
                                                             0, 0, LR_SHARED | LR_DEFAULTSIZE));
            wcex.hIconSm = m_icon.get()
                               ? m_icon.get()
                               : static_cast<HICON>(LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON,
                                                               0, 0, LR_SHARED | LR_DEFAULTSIZE));

            if (RegisterClassExA(&wcex) == 0)
                throw std::runtime_error("Class registration failure");
        }

        if (CreateWindowExA(dwExStyle, wcex.lpszClassName, wcex.lpszClassName, dwStyle, x, y,
                            nWidth, nHeight, hwndParent, hMenu, GetModuleHandleA(nullptr),
                            this) == nullptr)
            throw std::runtime_error("Window creation failure");
    }

    auto hwnd() const -> HWND { return m_hwnd.get(); }

    auto close() -> int
    {
        m_hwnd.reset();

        return 0;
    }

    auto notify(HWND hWnd, UINT code) -> void
    {
        NMHDR nmhdr;
        nmhdr.code = code;
        nmhdr.hwndFrom = hwnd();
        nmhdr.idFrom = id();
        SendMessageA(hWnd, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));
    }

    auto id() const -> int64_t { return m_id; }

    auto dpi() -> int64_t { return GetDpiForWindow(hwnd()); };

    auto scale() -> float
    {
        return static_cast<float>(dpi()) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
    };

    auto reveal() -> void { ShowWindow(hwnd(), SW_SHOWNORMAL); }

    auto show() -> void { ShowWindow(hwnd(), SW_SHOW); }

    auto hide() -> void { ShowWindow(hwnd(), SW_HIDE); }

    auto maximize() -> bool
    {
        auto style{GetWindowLongPtrA(hwnd(), GWL_STYLE)};

        WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
        GetWindowPlacement(hwnd(), &wp);

        if ((style & WS_OVERLAPPEDWINDOW) && wp.showCmd == 3)
        {
            ShowWindow(hwnd(), SW_SHOWNORMAL);

            return false;
        }

        else
        {
            ShowWindow(hwnd(), SW_MAXIMIZE);

            return true;
        }
    }

    auto fullscreen() -> bool
    {
        static RECT pos;

        auto style{GetWindowLongPtrA(hwnd(), GWL_STYLE)};

        if (style & WS_OVERLAPPEDWINDOW)
        {
            MONITORINFO mi{sizeof(mi)};
            GetWindowRect(hwnd(), &pos);
            if (GetMonitorInfoA(MonitorFromWindow(hwnd(), MONITOR_DEFAULTTONEAREST), &mi))
            {
                SetWindowLongPtrA(hwnd(), GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos(hwnd(), HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                             mi.rcMonitor.right - mi.rcMonitor.left,
                             mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED);
            }

            return true;
        }

        else
        {
            SetWindowLongPtrA(hwnd(), GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd(), nullptr, pos.left, pos.top, (pos.right - pos.left),
                         (pos.bottom - pos.top), SWP_FRAMECHANGED);

            return false;
        }
    }

    auto topmost() -> bool
    {
        FLASHWINFO fwi{sizeof(FLASHWINFO)};
        fwi.hwnd = hwnd();
        fwi.dwFlags = FLASHW_CAPTION;
        fwi.uCount = 1;
        fwi.dwTimeout = 100;

        auto style{GetWindowLongPtrA(hwnd(), GWL_EXSTYLE)};

        if (style & WS_EX_TOPMOST)
        {
            SetWindowPos(hwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            FlashWindowEx(&fwi);

            return false;
        }

        else
        {
            SetWindowPos(hwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            FlashWindowEx(&fwi);

            return true;
        }
    }

    auto title(std::string title) -> void { SetWindowTextA(hwnd(), title.c_str()); }

    auto icon(HICON icon) -> void
    {
        SendMessageA(hwnd(), WM_SETICON, ICON_SMALL, std::bit_cast<LPARAM>(icon));
        SendMessageA(hwnd(), WM_SETICON, ICON_BIG, std::bit_cast<LPARAM>(icon));
    }

    auto border(bool enabled) -> void
    {
        auto style{GetWindowLongPtrA(hwnd(), GWL_STYLE)};

        SetWindowLongPtrA(hwnd(), GWL_STYLE, enabled ? (style | WS_BORDER) : (style & ~WS_BORDER));
        SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto overlapped() -> void
    {
        SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
        SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto popup() -> void
    {
        SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
        SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto child() -> void
    {
        SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_CHILD);
        SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto reparent(HWND parent) -> void
    {
        if (parent)
        {
            SetParent(hwnd(), parent);
            child();
        }

        else
        {
            SetParent(hwnd(), nullptr);
            popup();
        }
    }

    auto client_rect() -> RECT
    {
        RECT rect{};
        GetClientRect(hwnd(), &rect);

        return rect;
    }

    auto window_rect() -> RECT
    {
        RECT rect{};
        GetWindowRect(hwnd(), &rect);

        return rect;
    }

    auto client_position() -> WindowPosition
    {
        auto rect{client_rect()};

        WindowPosition p;

        p.x = rect.left;
        p.y = rect.top;
        p.width = rect.right - rect.left;
        p.height = rect.bottom - rect.top;

        return p;
    }

    auto window_position() -> WindowPosition
    {
        auto rect{window_rect()};

        WindowPosition p;

        p.x = rect.left;
        p.y = rect.top;
        p.width = rect.right - rect.left;
        p.height = rect.bottom - rect.top;

        return p;
    }

    auto dwm_dark_mode(bool enabled) -> void
    {
        if (enabled)
        {
            auto useImmersiveDarkMode{TRUE};
            DwmSetWindowAttribute(hwnd(), DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
                                  sizeof(useImmersiveDarkMode));
        }

        else
        {
            auto useImmersiveDarkMode{FALSE};
            DwmSetWindowAttribute(hwnd(), DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
                                  sizeof(useImmersiveDarkMode));
        }
    }

    auto dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE backdrop) -> void
    {
        // MARGINS m{-1};
        MARGINS m{0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
        if (FAILED(DwmExtendFrameIntoClientArea(m_hwnd.get(), &m))) return;

        if (FAILED(DwmSetWindowAttribute(m_hwnd.get(), DWMWA_SYSTEMBACKDROP_TYPE, &backdrop,
                                         sizeof(&backdrop))))
            return;
    }

    auto dwm_rounded_corners(bool enable) -> void
    {
        if (enable)
        {
            auto corner{DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_ROUND};
            DwmSetWindowAttribute(hwnd(), DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
        }

        else
        {
            auto corner{DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DONOTROUND};
            DwmSetWindowAttribute(hwnd(), DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
        }
    }

    auto dwm_cloak(bool enable) -> void
    {
        if (enable)
        {
            auto cloak{TRUE};
            DwmSetWindowAttribute(hwnd(), DWMWA_CLOAK, &cloak, sizeof(cloak));
        }

        else
        {
            auto cloak{FALSE};
            DwmSetWindowAttribute(hwnd(), DWMWA_CLOAK, &cloak, sizeof(cloak));
        }
    }

    auto dwm_caption_color(bool enable) -> void
    {
        if (enable)
        {
            auto captionColor{DWMWA_COLOR_DEFAULT};
            DwmSetWindowAttribute(hwnd(), DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));
        }

        else
        {
            auto captionColor{DWMWA_COLOR_NONE};
            DwmSetWindowAttribute(hwnd(), DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));
        }
    }

    auto dwm_set_caption_color(COLORREF color) -> void
    {
        DwmSetWindowAttribute(hwnd(), DWMWA_CAPTION_COLOR, &color, sizeof(color));
    }

    auto dwm_border_color(bool enable) -> void
    {
        if (enable)
        {
            auto borderColor{DWMWA_COLOR_DEFAULT};
            DwmSetWindowAttribute(hwnd(), DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
        }

        else
        {
            auto borderColor{DWMWA_COLOR_NONE};
            DwmSetWindowAttribute(hwnd(), DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
        }
    }

    auto dwm_set_border_color(COLORREF color) -> void
    {
        DwmSetWindowAttribute(hwnd(), DWMWA_BORDER_COLOR, &color, sizeof(color));
    }

    auto dwm_set_text_color(COLORREF color) -> void
    {
        DwmSetWindowAttribute(hwnd(), DWMWA_TEXT_COLOR, &color, sizeof(color));
    }

    auto dwm_reset_text_color() -> void
    {
        auto textColor{DWMWA_COLOR_DEFAULT};
        DwmSetWindowAttribute(hwnd(), DWMWA_TEXT_COLOR, &textColor, sizeof(textColor));
    }

  private:
    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        T* self{nullptr};

        if (uMsg == WM_NCCREATE)
        {
            auto lpCreateStruct{std::bit_cast<LPCREATESTRUCT>(lParam)};
            self = static_cast<T*>(lpCreateStruct->lpCreateParams);
            self->m_hwnd.reset(hWnd);
            SetWindowLongPtrA(hWnd, 0, std::bit_cast<LONG_PTR>(self));
        }

        else self = std::bit_cast<T*>(GetWindowLongPtrA(hWnd, 0));

        if (self) { return self->handle_wnd_proc(uMsg, wParam, lParam); }

        else { return DefWindowProcA(hWnd, uMsg, wParam, lParam); }
    }

    WindowPosition m_windowPosition;
    SystemColors m_colors;

    wil::unique_hwnd m_hwnd;
    int64_t m_id{glow::text::random_int64()};
    wil::unique_hicon m_icon{static_cast<HICON>(LoadImageA(
        GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};
};

// struct Window
// {
//     Window(std::string className = "Window", bool show = true);
//     virtual ~Window();
//     virtual auto register_window() -> void;
//     virtual auto create_window() -> void;
//     virtual auto operator()(bool show = true) -> void;
//     static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
//     virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
//     virtual auto on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
//     auto dpi() -> int64_t;
//     auto scale() -> float;
//     auto show_normal() -> void;
//     auto show() -> void;
//     auto hide() -> void;
//     auto maximize() -> bool;
//     auto fullscreen() -> bool;
//     auto topmost() -> bool;
//     auto title(std::string title) -> void;
//     auto icon(HICON hIcon) -> void;
//     auto border(bool enabled) -> void;
//     auto overlapped() -> void;
//     auto popup() -> void;
//     auto child() -> void;
//     auto reparent(HWND parent) -> void;
//     auto client_rect() -> RECT;
//     auto window_rect() -> RECT;
//     auto client_position() -> Position;
//     auto window_position() -> Position;
//     auto dwm_dark_mode(bool enabled) -> void;
//     auto dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE backdrop) -> void;
//     auto dwm_rounded_corners(bool enabled) -> void;
//     auto dwm_cloak(bool enable) -> void;
//     auto dwm_caption_color(bool enable) -> void;
//     auto dwm_set_caption_color(COLORREF color) -> void;
//     auto dwm_border_color(bool enable) -> void;
//     auto dwm_set_border_color(COLORREF color) -> void;
//     auto dwm_set_text_color(COLORREF color) -> void;
//     auto dwm_reset_text_color() -> void;
//     std::string m_className{"Window"};
//     bool m_show{true};
//     WNDCLASSEXA m_wcex{sizeof(WNDCLASSEXA)};
//     wil::unique_hwnd m_hwnd{nullptr};
//     int64_t m_id{glow::text::random_int64()};
//     int64_t m_dpi{};
//     float m_scale{};
//     Position m_position;
//     Colors m_colors;
//     wil::unique_hcursor m_hCursor{static_cast<HCURSOR>(
//         LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
//     wil::unique_hicon m_hIcon{static_cast<HICON>(
//         LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
//     wil::unique_hicon m_appIcon{static_cast<HICON>(LoadImageA(
//         GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};
//     wil::unique_hbrush m_hbrBackgroundNull{static_cast<HBRUSH>(GetStockObject(NULL_BRUSH))};
//     wil::unique_hbrush m_hbrBackgroundBlack{static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))};
//     wil::unique_hbrush m_hbrBackgroundWhite{static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH))};
// };

// struct MainWindow : public Window
// {
//     MainWindow(std::string className = "MainWindow", bool show = true);
//     virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//         -> LRESULT override;
//     virtual auto on_destroy(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
// };

// struct WebView : public Window
// {
//     struct WebView2
//     {
//         wil::com_ptr<ICoreWebView2EnvironmentOptions6> m_evironmentOptions6{nullptr};
//         wil::com_ptr<ICoreWebView2Controller> m_controller{nullptr};
//         wil::com_ptr<ICoreWebView2Controller4> m_controller4{nullptr};
//         wil::com_ptr<ICoreWebView2> m_core{nullptr};
//         wil::com_ptr<ICoreWebView2_20> m_core20{nullptr};
//         wil::com_ptr<ICoreWebView2Settings> m_settings{nullptr};
//         wil::com_ptr<ICoreWebView2Settings8> m_settings8{nullptr};
//     };
//     WebView(int64_t id, HWND parent, std::string url = "about:blank",
//             std::string className = "WebView", bool show = true);
//     virtual auto operator()(bool show = true) -> void override;
//     virtual auto register_window() -> void override;
//     virtual auto create_window() -> void override;
//     auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
//     virtual auto on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
//     virtual auto create_webview() -> HRESULT;
//     virtual auto configure_settings() -> HRESULT;
//     virtual auto create_controller(ICoreWebView2Environment* createdEnvironment) -> HRESULT;
//     virtual auto initialized() -> void {}
//     auto context_menu_requested() -> HRESULT;
//     virtual auto context_menu_requested_handler(ICoreWebView2* sender,
//                                                 ICoreWebView2ContextMenuRequestedEventArgs* args)
//         -> HRESULT
//     {
//         return S_OK;
//     }
//     auto source_changed() -> HRESULT;
//     virtual auto source_changed_handler(ICoreWebView2* sender,
//                                         ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
//     {
//         return S_OK;
//     }
//     auto navigation_starting() -> HRESULT;
//     virtual auto navigation_starting_handler(ICoreWebView2* sender,
//                                              ICoreWebView2NavigationStartingEventArgs* args)
//         -> HRESULT
//     {
//         return S_OK;
//     }
//     auto navigation_completed() -> HRESULT;
//     virtual auto navigation_completed_handler(ICoreWebView2* sender,
//                                               ICoreWebView2NavigationCompletedEventArgs* args)
//         -> HRESULT
//     {
//         return S_OK;
//     }
//     auto web_message_received() -> HRESULT;
//     virtual auto web_message_received_handler(ICoreWebView2* sender,
//                                               ICoreWebView2WebMessageReceivedEventArgs* args)
//         -> HRESULT
//     {
//         return S_OK;
//     }
//     auto document_title_changed() -> HRESULT;
//     virtual auto document_title_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
//     {
//         return S_OK;
//     }
//     auto favicon_changed() -> HRESULT;
//     virtual auto favicon_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
//     {
//         return S_OK;
//     }
//     auto accelerator_key_pressed() -> HRESULT;
//     virtual auto accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
//                                                  ICoreWebView2AcceleratorKeyPressedEventArgs*
//                                                  args)
//         -> HRESULT
//     {
//         return S_OK;
//     }
//     auto zoom_factor_changed() -> HRESULT;
//     virtual auto zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
//         -> HRESULT
//     {
//         return S_OK;
//     }
//     auto navigate(std::string url) -> HRESULT;
//     auto post_json(const nlohmann::json jsonMessage) -> HRESULT;
//     HWND m_parent{nullptr};
//     std::string m_url{"about:blank"};
//     bool m_initialized{false};
//     WebView2 m_webView;
// };
} // namespace gui
} // namespace glow