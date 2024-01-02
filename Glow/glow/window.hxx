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
#include <ShlObj.h>
#include <wrl.h>

#include <wil/com.h>
#include <wil/result.h>
#include <wil/resource.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <algorithm>
#include <bit>
#include <memory>

namespace glow::window
{

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

struct Position
{
    int x{};
    int y{};
    int width{};
    int height{};
};

struct GdiPlus
{
    GdiPlus();
    ~GdiPlus();

    Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
    ULONG_PTR m_gdiplusToken{};
    Gdiplus::Status m_gdiplusStatus;
};

struct CoInitialize
{
    CoInitialize();
    ~CoInitialize();

    operator HRESULT() const;
    HRESULT m_result{};
};

struct Window
{
    Window();
    virtual ~Window();

    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;

    virtual auto on_close(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> int;
    virtual auto on_destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> int;

    inline static ATOM m_atom;
    wil::unique_hwnd m_hwnd{};

    wil::unique_hcursor m_hCursor{static_cast<HCURSOR>(
        LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    wil::unique_hicon m_hIcon{static_cast<HICON>(
        LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    wil::unique_hicon m_appIcon{static_cast<HICON>(LoadImageA(
        GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};
    wil::unique_hbrush m_hbrBackground{static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))};
};

struct MainWindow : public Window
{
    using Window::Window;

    auto on_destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> int;
};

auto message_loop() -> int;

auto get_dpi(HWND hwnd) -> int;
auto get_scale(HWND hwnd) -> float;

auto rect_to_position(RECT rect) -> Position;
auto client_rect(HWND hwnd) -> RECT;
auto window_rect(HWND hwnd) -> RECT;

auto is_dark() -> bool;

auto use_immersive_dark_mode(HWND hwnd) -> void;
auto set_system_backdrop(HWND hwnd, DWM_SYSTEMBACKDROP_TYPE backdrop) -> void;
auto set_rounded_corners(HWND hwnd, bool enabled) -> void;
auto cloak(HWND hwnd, bool enable) -> void;

auto clamp_color(int value) -> int;
auto make_colorref(int r, int g, int b) -> COLORREF;
auto format_color(winrt::Windows::UI::ViewManagement::UIColorType colorType) -> std::string;

auto enable_caption_color(HWND hwnd, bool enable) -> void;
auto enable_border_color(HWND hwnd, bool enable) -> void;

auto set_caption_color(HWND hwnd, COLORREF color) -> void;
auto set_border_color(HWND hwnd, COLORREF color) -> void;

auto reset_text_color(HWND hwnd) -> void;
auto set_text_color(HWND hwnd, COLORREF color) -> void;

auto set_preferred_app_mode() -> void;
auto allow_dark_mode(HWND hwnd, bool enable) -> void;

auto window_maximize(HWND hwnd) -> bool;
auto window_fullscreen(HWND hwnd) -> bool;
auto window_topmost(HWND hwnd) -> bool;

auto show_normal(HWND hwnd) -> void;
auto show(HWND hwnd) -> void;
auto hide(HWND hwnd) -> void;

auto set_title(HWND hwnd, std::string title) -> void;

auto icon_application() -> HICON;
auto icon_error() -> HICON;
auto icon_question() -> HICON;
auto icon_warning() -> HICON;
auto icon_information() -> HICON;
auto icon_winlogo() -> HICON;
auto icon_shield() -> HICON;

auto set_icon(HWND hwnd, HICON hIcon) -> void;
auto set_border(HWND hwnd, bool enabled) -> void;
auto set_child(HWND hwnd) -> void;
auto set_popup(HWND hwnd) -> void;
auto set_overlapped(HWND hwnd) -> void;

} // namespace glow::window
