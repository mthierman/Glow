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

#include <text/text.hxx>
#include <gui/window.hxx>

namespace glow::gui
{

using json = nlohmann::json;

struct WebView2
{
    WebView2(std::string, HWND parentHwnd, int64_t id);
    virtual ~WebView2();

    auto create() -> void;

    auto create_environment() -> void;
    auto create_controller(ICoreWebView2Environment* environment) -> void;

    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto on_close() -> int;
    virtual auto on_size() -> int;

    auto navigate(std::string url) -> void;
    auto post_json(const json jsonMessage) -> void;

    auto source_changed() -> void;
    auto navigation_completed() -> void;
    auto web_message_received() -> void;
    auto accelerator_key_pressed() -> void;
    auto favicon_changed() -> void;
    auto document_title_changed() -> void;
    auto frame_navigation_starting() -> void;

    auto initialize() -> void;

    virtual auto source_changed_handler() -> void{};
    virtual auto web_message_received_handler() -> void{};
    virtual auto accelerator_key_pressed_handler(ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> void{};
    virtual auto favicon_changed_handler() -> void{};
    virtual auto document_title_changed_handler() -> void{};

    inline static ATOM m_atom;
    wil::unique_hwnd m_hwnd{};

    std::string m_url;
    wil::unique_hwnd m_hwndParent;
    int64_t m_id{};
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