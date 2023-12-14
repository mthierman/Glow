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
#include <WebView2EnvironmentOptions.h>

#include <string>

#include <nlohmann/json.hpp>
#include <gui/gui.hxx>
#include <text/text.hxx>

namespace glow::gui
{
using json = nlohmann::json;
//==============================================================================
struct WebView
{
    WebView(std::string name, HWND parentHwnd, int id);
    virtual ~WebView();

    //==============================================================================
    auto register_window() -> ATOM;
    auto create_window() -> void;
    auto show_window_default() -> void;
    auto show_window() -> void;
    auto hide_window() -> void;

    //==============================================================================
    auto navigate(const std::string url) -> void;
    auto post_json(const json jsonMessage) -> void;

    //==============================================================================
    static auto CALLBACK wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto on_window_pos_changed() -> int;
    virtual auto handle_message(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;

    //==============================================================================
    auto create_environment() -> void;
    auto create_controller(ICoreWebView2Environment* environment) -> void;

    //==============================================================================
    virtual auto source_changed() -> void;
    virtual auto navigation_completed() -> void;
    virtual auto web_message_received() -> void;
    virtual auto accelerator_key_pressed() -> void;
    virtual auto favicon_changed() -> void;
    virtual auto document_title_changed() -> void;

    //==============================================================================
    auto initialized() -> void;

    //==============================================================================
    virtual auto source_changed_handler() -> void{};
    virtual auto web_message_received_handler() -> void{};
    virtual auto accelerator_key_pressed_handler(ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> void{};
    virtual auto favicon_changed_handler() -> void{};
    virtual auto document_title_changed_handler() -> void{};

    //==============================================================================
    wil::unique_hwnd m_hwnd;
    wil::unique_hwnd m_hwndParent;
    winrt::com_ptr<ICoreWebView2EnvironmentOptions6> m_evironmentOptions6{nullptr};
    winrt::com_ptr<ICoreWebView2Controller> m_controller{nullptr};
    winrt::com_ptr<ICoreWebView2Controller4> m_controller4{nullptr};
    winrt::com_ptr<ICoreWebView2> m_core{nullptr};
    winrt::com_ptr<ICoreWebView2_20> m_core20{nullptr};
    winrt::com_ptr<ICoreWebView2Settings> m_settings{nullptr};
    winrt::com_ptr<ICoreWebView2Settings8> m_settings8{nullptr};
    HCURSOR m_cursor{
        reinterpret_cast<HCURSOR>(LoadImageA(nullptr, reinterpret_cast<LPCSTR>(IDC_ARROW),
                                             IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    HICON m_defaultIcon{
        reinterpret_cast<HICON>(LoadImageA(nullptr, reinterpret_cast<LPCSTR>(IDI_APPLICATION),
                                           IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    wil::unique_hicon m_icon{reinterpret_cast<HICON>(LoadImageA(
        GetModuleHandleA(nullptr), MAKEINTRESOURCE(1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};
    HBRUSH m_background{reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))};
    std::string m_name;
    std::string m_class;
    ATOM m_classAtom{};
    UINT_PTR m_id{0};
    bool m_initialized{false};
};

//==============================================================================
} // namespace glow::gui
