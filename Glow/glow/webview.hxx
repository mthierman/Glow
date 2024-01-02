// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <wrl.h>

#include <wil/com.h>
#include <wil/resource.h>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <nlohmann/json.hpp>

#include <glow/text.hxx>
#include <glow/window.hxx>

namespace glow::webview
{
using json = nlohmann::json;

struct WebView2
{
    WebView2(HWND hwndParent, int64_t id, std::string = "https://www.google.com/");
    virtual ~WebView2();

    auto show_normal() -> void;
    auto show() -> void;
    auto hide() -> void;

    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto on_close() -> int;
    virtual auto on_size() -> int;

    auto create_environment() -> void;
    auto create_controller(ICoreWebView2Environment* environment) -> void;
    virtual auto initialized() -> void{};

    auto navigate(std::string url) -> void;
    auto post_json(const json jsonMessage) -> void;

    auto source_changed() -> void;
    virtual auto source_changed_handler() -> void{};

    auto navigation_completed() -> void;
    virtual auto navigation_completed_handler() -> void{};

    auto web_message_received() -> void;
    virtual auto web_message_received_handler() -> void{};

    auto accelerator_key_pressed() -> void;
    virtual auto accelerator_key_pressed_handler(ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> void{};

    auto favicon_changed() -> void;
    virtual auto favicon_changed_handler() -> void{};

    auto document_title_changed() -> void;
    virtual auto document_title_changed_handler() -> void{};

    HWND m_hwndParent;
    int64_t m_id{};
    std::string m_url;

    inline static ATOM m_atom;
    wil::unique_hwnd m_hwnd{};

    bool m_initialized{false};

    wil::com_ptr<ICoreWebView2EnvironmentOptions6> m_evironmentOptions6{nullptr};
    wil::com_ptr<ICoreWebView2Controller> m_controller{nullptr};
    wil::com_ptr<ICoreWebView2Controller4> m_controller4{nullptr};
    wil::com_ptr<ICoreWebView2> m_core{nullptr};
    wil::com_ptr<ICoreWebView2_20> m_core20{nullptr};
    wil::com_ptr<ICoreWebView2Settings> m_settings{nullptr};
    wil::com_ptr<ICoreWebView2Settings8> m_settings8{nullptr};

    wil::unique_hcursor m_hCursor{static_cast<HCURSOR>(
        LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    wil::unique_hicon m_hIcon{static_cast<HICON>(
        LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    wil::unique_hicon m_appIcon{static_cast<HICON>(LoadImageA(
        GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};
    wil::unique_hbrush m_hbrBackground{static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))};
};

} // namespace glow::gui
