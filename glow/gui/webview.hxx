// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <wrl.h>

#include <winrt/Windows.Foundation.h>

#include <WebView2.h>

#include <string>

#include <gui/gui.hxx>
#include <text/text.hxx>

//==============================================================================
namespace glow::gui
{

//==============================================================================
struct WebView
{
    WebView(std::string name, HWND parentHwnd, int id);
    virtual ~WebView();

    //==============================================================================
    std::string m_name;
    std::string m_class;
    ATOM m_classAtom{};
    wil::unique_hwnd m_hwnd;
    wil::unique_hwnd m_hwndParent;

  private:
    auto register_window() -> ATOM;
    auto create_window() -> void;

    //==============================================================================
    auto show_window_default() -> void;
    auto show_window() -> void;
    auto hide_window() -> void;

    //==============================================================================
    static auto CALLBACK wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto handle_message(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;

    //==============================================================================
    virtual auto on_window_pos_changed() -> int;

    //==============================================================================
    auto create_environment() -> void;
    auto create_controller(ICoreWebView2Environment* environment) -> void;
    auto navigation_completed() -> void;

    //==============================================================================

    HCURSOR m_cursor{
        reinterpret_cast<HCURSOR>(::LoadImage(nullptr, reinterpret_cast<LPCSTR>(IDC_ARROW),
                                              IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    HICON m_defaultIcon{
        reinterpret_cast<HICON>(::LoadImage(nullptr, reinterpret_cast<LPCSTR>(IDI_APPLICATION),
                                            IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    HICON m_icon{reinterpret_cast<HICON>(::LoadImage(::GetModuleHandle(nullptr), MAKEINTRESOURCE(1),
                                                     IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};
    HBRUSH m_background{reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH))};

    //==============================================================================
    UINT_PTR m_id{0};
    bool m_initialized{false};
    winrt::com_ptr<ICoreWebView2Controller> m_controller{nullptr};
    winrt::com_ptr<ICoreWebView2Controller4> m_controller4{nullptr};
    winrt::com_ptr<ICoreWebView2> m_core{nullptr};
    winrt::com_ptr<ICoreWebView2_19> m_core19{nullptr};
    winrt::com_ptr<ICoreWebView2Settings> m_settings{nullptr};
    winrt::com_ptr<ICoreWebView2Settings8> m_settings8{nullptr};
};

//==============================================================================
} // namespace glow::gui
