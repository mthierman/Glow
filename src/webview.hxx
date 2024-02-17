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
#include <objbase.h>
#include <gdiplus.h>

#include <expected>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>

#include <wil/com.h>
#include <wil/resource.h>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <nlohmann/json.hpp>

#include "random.hxx"
#include "text.hxx"
#include "window.hxx"

namespace glow
{
struct WebView : public Window
{
    WebView(
        ::HWND parent, std::function<::HRESULT()> callback = []() { return S_OK; },
        size_t id = glow::random<size_t>());
    ~WebView() override;

    virtual auto create_webview(std::function<::HRESULT()> callback) -> ::HRESULT;
    virtual auto put_settings() -> ::HRESULT;
    virtual auto add_context_menu_requested() -> ::HRESULT;
    virtual auto context_menu_requested_handler(ICoreWebView2* sender,
                                                ICoreWebView2ContextMenuRequestedEventArgs* args)
        -> ::HRESULT;
    virtual auto add_source_changed() -> ::HRESULT;
    virtual auto source_changed_handler(ICoreWebView2* sender,
                                        ICoreWebView2SourceChangedEventArgs* args) -> ::HRESULT;
    virtual auto add_navigation_starting() -> ::HRESULT;
    virtual auto navigation_starting_handler(ICoreWebView2* sender,
                                             ICoreWebView2NavigationStartingEventArgs* args)
        -> ::HRESULT;
    virtual auto add_navigation_completed() -> ::HRESULT;
    virtual auto navigation_completed_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationCompletedEventArgs* args)
        -> ::HRESULT;
    virtual auto add_web_message_received() -> ::HRESULT;
    virtual auto web_message_received_handler(ICoreWebView2* sender,
                                              ICoreWebView2WebMessageReceivedEventArgs* args)
        -> ::HRESULT;
    virtual auto add_document_title_changed() -> ::HRESULT;
    virtual auto document_title_changed_handler(ICoreWebView2* sender, IUnknown* args) -> ::HRESULT;
    virtual auto add_favicon_changed() -> ::HRESULT;
    virtual auto favicon_changed_handler(ICoreWebView2* sender, IUnknown* args) -> ::HRESULT;
    virtual auto add_accelerator_key_pressed() -> ::HRESULT;
    virtual auto accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                                 ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> ::HRESULT;
    virtual auto add_zoom_factor_changed() -> ::HRESULT;
    virtual auto zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
        -> ::HRESULT;
    virtual auto add_got_focus() -> ::HRESULT;
    virtual auto got_focus_handler(ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT;
    virtual auto add_lost_focus() -> ::HRESULT;
    virtual auto lost_focus_handler(ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT;
    auto add_move_focus_requested() -> ::HRESULT;
    virtual auto move_focus_requested_handler(ICoreWebView2Controller* sender,
                                              ICoreWebView2MoveFocusRequestedEventArgs* args)
        -> ::HRESULT;
    virtual auto navigate(std::string url) -> ::HRESULT;
    virtual auto post_json(nlohmann::json message) -> ::HRESULT;
    virtual auto get_favicon(std::function<::HRESULT()> callback = []() { return S_OK; })
        -> ::HRESULT;
    virtual auto open_dev_tools_window() -> ::HRESULT;
    virtual auto
    move_focus(COREWEBVIEW2_MOVE_FOCUS_REASON reason = COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC)
        -> ::HRESULT;
    virtual auto put_zoom_factor(double zoomFactor) -> ::HRESULT;
    virtual auto put_is_visible(bool visible) -> ::HRESULT;
    virtual auto webview_version() -> std::expected<std::string, ::HRESULT>;

    std::string m_source{};
    std::optional<std::string> m_title{};
    std::pair<std::string, wil::unique_hicon> m_favicon{};

  private:
    auto WndProc(::UINT message, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT override;
    auto on_create(::WPARAM wParam, ::LPARAM lParam) -> int override;
    virtual auto on_size(::WPARAM wParam, ::LPARAM lParam) -> int;

    std::function<::HRESULT()> m_callback;



    wil::com_ptr<ICoreWebView2Controller> m_initController{};
    wil::com_ptr<ICoreWebView2> m_initCore{};
    wil::com_ptr<ICoreWebView2Settings> m_initSettings{};

    Microsoft::WRL::ComPtr<CoreWebView2EnvironmentOptions> m_environmentOptions;
    wil::com_ptr<ICoreWebView2Controller4> m_controller{};
    wil::com_ptr<ICoreWebView2_21> m_core{};
    wil::com_ptr<ICoreWebView2Settings8> m_settings{};

    ::EventRegistrationToken m_tokenContextMenuRequested{};
    ::EventRegistrationToken m_tokenSourceChanged{};
    ::EventRegistrationToken m_tokenNavigationStarting{};
    ::EventRegistrationToken m_tokenNavigationCompleted{};
    ::EventRegistrationToken m_tokenWebMessageReceived{};
    ::EventRegistrationToken m_tokenDocumentTitleChanged{};
    ::EventRegistrationToken m_tokenFaviconChanged{};
    ::EventRegistrationToken m_tokenAcceleratorKeyPressed{};
    ::EventRegistrationToken m_tokenZoomFactorChanged{};
    ::EventRegistrationToken m_tokenGotFocus{};
    ::EventRegistrationToken m_tokenLostFocus{};
    ::EventRegistrationToken m_tokenMoveFocusRequested{};
};
} // namespace glow
