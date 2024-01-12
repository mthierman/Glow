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
struct Console
{
    Console();
    ~Console();
    FILE* pFile{nullptr};
};

auto argv() -> std::vector<std::string>;
auto hresult_string(HRESULT errorCode) -> std::string;
auto hresult_check(HRESULT errorCode) -> HRESULT;
auto hresult_debug(HRESULT errorCode,
                   std::source_location location = std::source_location::current()) -> void;
auto hresult_print(HRESULT errorCode,
                   std::source_location location = std::source_location::current()) -> void;
auto source_debug(std::string message,
                  std::source_location location = std::source_location::current()) -> void;
auto source_print(std::string message,
                  std::source_location location = std::source_location::current()) -> void;
auto message_box(std::string message, UINT type = MB_OK | MB_ICONINFORMATION) -> int;
auto message_box_shell(std::string message, UINT type = MB_OK | MB_ICONINFORMATION) -> int;
auto message_box_stock(std::string message, SHSTOCKICONID icon = SIID_INFO) -> void;
auto create_process(std::string process) -> int;
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
auto app_name() -> std::string;
auto path_portable() -> std::filesystem::path;
auto path_portable_wide() -> std::filesystem::path;
} // namespace filesystem

namespace text
{
auto narrow(std::wstring utf16) -> std::string;
auto widen(std::string utf8) -> std::wstring;
auto randomize(std::string string) -> std::string;
auto random_int64() -> int64_t;
auto random_int32() -> int32_t;
auto create_guid() -> GUID;
auto guid_string(GUID guid) -> std::string;
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

struct WebView2
{
    WebView2();

    wil::com_ptr<ICoreWebView2EnvironmentOptions6> evironmentOptions6;
    wil::com_ptr<ICoreWebView2Controller> controller;
    wil::com_ptr<ICoreWebView2Controller4> controller4;
    wil::com_ptr<ICoreWebView2> core;
    wil::com_ptr<ICoreWebView2_20> core20;
    wil::com_ptr<ICoreWebView2Settings> settings;
    wil::com_ptr<ICoreWebView2Settings8> settings8;
};

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
    MessageWindow(std::string name = "MessageWindow", int64_t id = glow::text::random_int64())
        : m_id{id}
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

        if (self) { return self->default_wnd_proc(hWnd, uMsg, wParam, lParam); }

        else { return DefWindowProcA(hWnd, uMsg, wParam, lParam); }
    }

    virtual auto default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        switch (uMsg)
        {
        case WM_CLOSE: return close();
        case WM_DESTROY: PostQuitMessage(0); return 0;
        }

        return wnd_proc(hWnd, uMsg, wParam, lParam);
    }

    virtual auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
    }

    int64_t m_id;
    wil::unique_hwnd m_hwnd;
};

template <typename T> struct BaseWindow
{
    BaseWindow(std::string name = "BaseWindow", int64_t id = glow::text::random_int64(),
               DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, DWORD dwExStyle = 0,
               int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT,
               int nHeight = CW_USEDEFAULT, HWND hwndParent = nullptr, HMENU hMenu = nullptr)
        : m_id{id}
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

        if (self) { return self->default_wnd_proc(hWnd, uMsg, wParam, lParam); }

        else { return DefWindowProcA(hWnd, uMsg, wParam, lParam); }
    }

    virtual auto default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        switch (uMsg)
        {
        case WM_CLOSE: return close();
        }

        return wnd_proc(hWnd, uMsg, wParam, lParam);
    }

    virtual auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
    }

  public:
    WindowPosition m_windowPosition;
    SystemColors m_colors;
    wil::unique_hwnd m_hwnd;
    int64_t m_id{};
    wil::unique_hicon m_icon{static_cast<HICON>(LoadImageA(
        GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};
};

template <typename T> struct WebView : BaseWindow<T>
{
    T* self{static_cast<T*>(this)};

    WebView(HWND parent, std::string url = "https://www.google.com/",
            int64_t id = glow::text::random_int64())
        : BaseWindow<T>("WebView", id, WS_CHILD, 0, 0, 0, 0, 0, parent, std::bit_cast<HMENU>(id))
    {
        m_parent = parent;
        m_url = url;
        glow::console::hresult_check(create());
    }

    auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override
    {
        switch (uMsg)
        {
        case WM_SIZE: return on_size(hWnd, wParam, lParam);
        }

        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
    }

    auto on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
    {
        if (m_webView.controller4)
        {
            RECT rect{};
            GetClientRect(hWnd, &rect);
            m_webView.controller4->put_Bounds(rect);
        }

        return 0;
    }

    auto create() -> HRESULT
    {
        return CreateCoreWebView2EnvironmentWithOptions(
            nullptr, nullptr, nullptr,
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [=, this](HRESULT errorCode,
                          ICoreWebView2Environment* createdEnvironment) -> HRESULT
                {
                    if (createdEnvironment)
                    {
                        glow::console::hresult_check(create_controller(createdEnvironment));
                    }

                    return errorCode;
                })
                .Get());
    }

    auto create_controller(ICoreWebView2Environment* createdEnvironment) -> HRESULT
    {
        return createdEnvironment->CreateCoreWebView2Controller(
            self->hwnd(),
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                [=, this](HRESULT errorCode, ICoreWebView2Controller* createdController) -> HRESULT
                {
                    if (createdController)
                    {
                        m_webView.controller = createdController;
                        m_webView.controller4 =
                            m_webView.controller.try_query<ICoreWebView2Controller4>();

                        if (!m_webView.controller4) return E_POINTER;

                        COREWEBVIEW2_COLOR bgColor{0, 0, 0, 0};
                        glow::console::hresult_check(
                            m_webView.controller4->put_DefaultBackgroundColor(bgColor));

                        SendMessageA(m_parent, WM_SIZE, 0, 0);
                        SendMessageA(self->hwnd(), WM_SIZE, 0, 0);

                        glow::console::hresult_check(
                            m_webView.controller->get_CoreWebView2(m_webView.core.put()));
                        m_webView.core20 = m_webView.core.try_query<ICoreWebView2_20>();

                        if (!m_webView.core20) return E_POINTER;

                        glow::console::hresult_check(
                            m_webView.core20->get_Settings(m_webView.settings.put()));

                        m_webView.settings8 =
                            m_webView.settings.try_query<ICoreWebView2Settings8>();

                        if (!m_webView.settings8) return E_POINTER;

                        glow::console::hresult_check(configure_settings());

                        glow::console::hresult_check(
                            m_webView.core20->Navigate(text::widen(m_url).c_str()));

                        glow::console::hresult_check(context_menu_requested());
                        glow::console::hresult_check(source_changed());
                        glow::console::hresult_check(navigation_starting());
                        glow::console::hresult_check(navigation_completed());
                        glow::console::hresult_check(web_message_received());
                        glow::console::hresult_check(accelerator_key_pressed());
                        glow::console::hresult_check(favicon_changed());
                        glow::console::hresult_check(document_title_changed());
                        glow::console::hresult_check(zoom_factor_changed());

                        if (!m_initialized)
                        {
                            m_initialized = true;
                            initialized();
                        }
                    }

                    return errorCode;
                })
                .Get());
    }

    auto configure_settings() -> HRESULT
    {
        glow::console::hresult_check(
            m_webView.settings8->put_AreBrowserAcceleratorKeysEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_AreDefaultContextMenusEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_AreDefaultScriptDialogsEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_AreDevToolsEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_AreHostObjectsAllowed(true));
        glow::console::hresult_check(m_webView.settings8->put_HiddenPdfToolbarItems(
            COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE));
        glow::console::hresult_check(m_webView.settings8->put_IsBuiltInErrorPageEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsGeneralAutofillEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsPasswordAutosaveEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsPinchZoomEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsReputationCheckingRequired(true));
        glow::console::hresult_check(m_webView.settings8->put_IsScriptEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsStatusBarEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsSwipeNavigationEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsWebMessageEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsZoomControlEnabled(true));

        return S_OK;
    }

    auto initialized() -> void {}

    auto context_menu_requested() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_ContextMenuRequested(
            Microsoft::WRL::Callback<ICoreWebView2ContextMenuRequestedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2ContextMenuRequestedEventArgs* args) -> HRESULT
                { return context_menu_requested_handler(sender, args); })
                .Get(),
            &token);
    }

    auto source_changed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_SourceChanged(
            Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
                { return source_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto navigation_starting() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_NavigationStarting(
            Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT
                { return navigation_starting_handler(sender, args); })
                .Get(),
            &token);
    }

    auto navigation_completed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_NavigationCompleted(
            Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
                { return navigation_completed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto web_message_received() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_WebMessageReceived(
            Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                { return web_message_received_handler(sender, args); })
                .Get(),
            &token);
    }

    auto document_title_changed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_DocumentTitleChanged(
            Microsoft::WRL::Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                [=, this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                { return document_title_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto favicon_changed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_FaviconChanged(
            Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                [=, this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                { return favicon_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto accelerator_key_pressed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.controller4->add_AcceleratorKeyPressed(
            Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
                [=, this](ICoreWebView2Controller* sender,
                          ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT
                { return accelerator_key_pressed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto zoom_factor_changed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.controller4->add_ZoomFactorChanged(
            Microsoft::WRL::Callback<ICoreWebView2ZoomFactorChangedEventHandler>(
                [=, this](ICoreWebView2Controller* sender, IUnknown* args) -> HRESULT
                { return zoom_factor_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    virtual auto context_menu_requested_handler(ICoreWebView2* sender,
                                                ICoreWebView2ContextMenuRequestedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }

    virtual auto source_changed_handler(ICoreWebView2* sender,
                                        ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
    {
        return S_OK;
    }

    virtual auto navigation_starting_handler(ICoreWebView2* sender,
                                             ICoreWebView2NavigationStartingEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }

    virtual auto navigation_completed_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationCompletedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }

    virtual auto web_message_received_handler(ICoreWebView2* sender,
                                              ICoreWebView2WebMessageReceivedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }

    virtual auto document_title_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
    {
        return S_OK;
    }

    virtual auto favicon_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
    {
        return S_OK;
    }

    virtual auto accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                                 ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }

    virtual auto zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
        -> HRESULT
    {
        return S_OK;
    }

    auto navigate(std::string url) -> HRESULT
    {
        auto wideUrl{glow::text::widen(url)};
        if (m_webView.core20)
            return glow::console::hresult_check(m_webView.core20->Navigate(wideUrl.c_str()));

        else return S_OK;
    }

    auto post_json(const nlohmann::json json) -> HRESULT
    {
        auto wideJson{glow::text::widen(json)};
        if (m_webView.core20)
            return glow::console::hresult_check(
                m_webView.core20->PostWebMessageAsJson(wideJson.c_str()));

        else return S_OK;
    }

    HWND m_parent{nullptr};
    std::string m_url;
    GdiPlus m_gdiInit;
    CoInitialize m_coInit;
    WebView2 m_webView;
    bool m_initialized{false};
};
} // namespace gui
} // namespace glow
