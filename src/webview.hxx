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
template <typename T> struct WebView : Window<T>
{
    struct WebView2
    {
        wil::com_ptr<ICoreWebView2EnvironmentOptions6> evironmentOptions6;
        wil::com_ptr<ICoreWebView2Controller> controller;
        wil::com_ptr<ICoreWebView2Controller4> controller4;
        wil::com_ptr<ICoreWebView2> core;
        wil::com_ptr<ICoreWebView2_20> core20;
        wil::com_ptr<ICoreWebView2Settings> settings;
        wil::com_ptr<ICoreWebView2Settings8> settings8;
    };

    WebView(HWND parent, intptr_t id = glow::random<intptr_t>())
        : Window<T>("WebView", id, WS_CHILD, 0, 0, 0, 0, 0, parent, reinterpret_cast<::HMENU>(id)),
          m_parent{parent}
    {
        if (FAILED(create())) { throw std::runtime_error("WebView creation failure"); }
    }

    virtual ~WebView() {}

    auto wnd_proc(::HWND hwnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT override
    {
        switch (message)
        {
        case WM_SIZE: return on_size(wParam, lParam);
        }

        return ::DefWindowProcA(hwnd, message, wParam, lParam);
    }

    auto on_size(::WPARAM wParam, ::LPARAM lParam) -> int
    {
        if (m_webView.controller4)
        {
            derived().position();
            m_webView.controller4->put_Bounds(derived().m_client.rect);
        }

        return 0;
    }

    auto create() -> ::HRESULT
    {
        return CreateCoreWebView2EnvironmentWithOptions(
            nullptr, nullptr, nullptr,
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [=, this](::HRESULT errorCode,
                          ICoreWebView2Environment* createdEnvironment) -> ::HRESULT
                {
                    if (createdEnvironment) { return create_controller(createdEnvironment); }

                    else { return errorCode; }
                })
                .Get());
    }

    auto create_controller(ICoreWebView2Environment* createdEnvironment) -> ::HRESULT
    {
        return createdEnvironment->CreateCoreWebView2Controller(
            derived().hwnd(),
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                [=, this](::HRESULT errorCode,
                          ICoreWebView2Controller* createdController) -> ::HRESULT
                {
                    if (createdController)
                    {
                        m_webView.controller = createdController;
                        m_webView.controller4 =
                            m_webView.controller.try_query<ICoreWebView2Controller4>();

                        if (!m_webView.controller4) return E_POINTER;

                        COREWEBVIEW2_COLOR bgColor{0, 0, 0, 0};
                        m_webView.controller4->put_DefaultBackgroundColor(bgColor);

                        ::SendMessageA(m_parent, WM_SIZE, 0, 0);
                        ::SendMessageA(derived().hwnd(), WM_SIZE, 0, 0);

                        m_webView.controller->get_CoreWebView2(m_webView.core.put());
                        m_webView.core20 = m_webView.core.try_query<ICoreWebView2_20>();

                        if (!m_webView.core20) return E_POINTER;

                        m_webView.core20->get_Settings(m_webView.settings.put());

                        m_webView.settings8 =
                            m_webView.settings.try_query<ICoreWebView2Settings8>();

                        if (!m_webView.settings8) return E_POINTER;

                        settings();

                        context_menu_requested();
                        source_changed();
                        navigation_starting();
                        navigation_completed();
                        web_message_received();
                        accelerator_key_pressed();
                        favicon_changed();
                        document_title_changed();
                        zoom_factor_changed();
                        got_focus();
                        lost_focus();
                        move_focus_requested();

                        initialized();

                        return S_OK;
                    }

                    else { return errorCode; }
                })
                .Get());
    }

    virtual auto settings() -> ::HRESULT
    {
        m_webView.settings8->put_AreBrowserAcceleratorKeysEnabled(true);
        m_webView.settings8->put_AreDefaultContextMenusEnabled(true);
        m_webView.settings8->put_AreDefaultScriptDialogsEnabled(true);
        m_webView.settings8->put_AreDevToolsEnabled(true);
        m_webView.settings8->put_AreHostObjectsAllowed(true);
        m_webView.settings8->put_HiddenPdfToolbarItems(
            COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE);
        m_webView.settings8->put_IsBuiltInErrorPageEnabled(true);
        m_webView.settings8->put_IsGeneralAutofillEnabled(true);
        m_webView.settings8->put_IsPasswordAutosaveEnabled(true);
        m_webView.settings8->put_IsPinchZoomEnabled(true);
        m_webView.settings8->put_IsReputationCheckingRequired(true);
        m_webView.settings8->put_IsScriptEnabled(true);
        m_webView.settings8->put_IsStatusBarEnabled(true);
        m_webView.settings8->put_IsSwipeNavigationEnabled(true);
        m_webView.settings8->put_IsWebMessageEnabled(true);
        m_webView.settings8->put_IsZoomControlEnabled(true);

        return S_OK;
    }

    virtual auto initialized() -> void {}

    auto context_menu_requested() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_ContextMenuRequested(
            Microsoft::WRL::Callback<ICoreWebView2ContextMenuRequestedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2ContextMenuRequestedEventArgs* args) -> ::HRESULT
                { return context_menu_requested_handler(sender, args); })
                .Get(),
            &token);
    }

    auto source_changed() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_SourceChanged(
            Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2SourceChangedEventArgs* args) -> ::HRESULT
                { return source_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto navigation_starting() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_NavigationStarting(
            Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2NavigationStartingEventArgs* args) -> ::HRESULT
                { return navigation_starting_handler(sender, args); })
                .Get(),
            &token);
    }

    auto navigation_completed() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_NavigationCompleted(
            Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2NavigationCompletedEventArgs* args) -> ::HRESULT
                { return navigation_completed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto web_message_received() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_WebMessageReceived(
            Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2WebMessageReceivedEventArgs* args) -> ::HRESULT
                { return web_message_received_handler(sender, args); })
                .Get(),
            &token);
    }

    auto document_title_changed() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_DocumentTitleChanged(
            Microsoft::WRL::Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                [=, this](ICoreWebView2* sender, IUnknown* args) -> ::HRESULT
                { return document_title_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto favicon_changed() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_FaviconChanged(
            Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                [=, this](ICoreWebView2* sender, IUnknown* args) -> ::HRESULT
                { return favicon_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto accelerator_key_pressed() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.controller4->add_AcceleratorKeyPressed(
            Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
                [=, this](ICoreWebView2Controller* sender,
                          ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> ::HRESULT
                { return accelerator_key_pressed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto zoom_factor_changed() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.controller4->add_ZoomFactorChanged(
            Microsoft::WRL::Callback<ICoreWebView2ZoomFactorChangedEventHandler>(
                [=, this](ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT
                { return zoom_factor_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto got_focus() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.controller4->add_GotFocus(
            Microsoft::WRL::Callback<ICoreWebView2FocusChangedEventHandler>(
                [=, this](ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT
                { return got_focus_handler(sender, args); })
                .Get(),
            &token);
    }

    auto lost_focus() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.controller4->add_LostFocus(
            Microsoft::WRL::Callback<ICoreWebView2FocusChangedEventHandler>(
                [=, this](ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT
                { return lost_focus_handler(sender, args); })
                .Get(),
            &token);
    }

    auto move_focus_requested() -> ::HRESULT
    {
        EventRegistrationToken token;

        return m_webView.controller4->add_MoveFocusRequested(
            Microsoft::WRL::Callback<ICoreWebView2MoveFocusRequestedEventHandler>(
                [=, this](ICoreWebView2Controller* sender,
                          ICoreWebView2MoveFocusRequestedEventArgs* args) -> ::HRESULT
                { return move_focus_requested_handler(sender, args); })
                .Get(),
            &token);
    }

    virtual auto context_menu_requested_handler(ICoreWebView2* sender,
                                                ICoreWebView2ContextMenuRequestedEventArgs* args)
        -> ::HRESULT
    {
        return S_OK;
    }

    virtual auto source_changed_handler(ICoreWebView2* sender,
                                        ICoreWebView2SourceChangedEventArgs* args) -> ::HRESULT
    {
        return S_OK;
    }

    virtual auto navigation_starting_handler(ICoreWebView2* sender,
                                             ICoreWebView2NavigationStartingEventArgs* args)
        -> ::HRESULT
    {
        return S_OK;
    }

    virtual auto navigation_completed_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationCompletedEventArgs* args)
        -> ::HRESULT
    {
        return S_OK;
    }

    virtual auto web_message_received_handler(ICoreWebView2* sender,
                                              ICoreWebView2WebMessageReceivedEventArgs* args)
        -> ::HRESULT
    {
        return S_OK;
    }

    virtual auto document_title_changed_handler(ICoreWebView2* sender, IUnknown* args) -> ::HRESULT
    {
        return S_OK;
    }

    virtual auto favicon_changed_handler(ICoreWebView2* sender, IUnknown* args) -> ::HRESULT
    {
        return S_OK;
    }

    virtual auto accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                                 ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> ::HRESULT
    {
        return S_OK;
    }

    virtual auto zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
        -> ::HRESULT
    {
        return S_OK;
    }

    virtual auto got_focus_handler(ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT
    {
        return S_OK;
    }

    virtual auto lost_focus_handler(ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT
    {
        return S_OK;
    }

    virtual auto move_focus_requested_handler(ICoreWebView2Controller* sender,
                                              ICoreWebView2MoveFocusRequestedEventArgs* args)
        -> ::HRESULT
    {
        return S_OK;
    }

    auto navigate(std::string url) -> ::HRESULT
    {
        if (m_webView.core20)
        {
            auto wideUrl{glow::wstring(url)};

            if (wideUrl.empty()) { return S_OK; }

            else return m_webView.core20->Navigate(wideUrl.c_str());
        }

        else return S_OK;
    }

    auto post_json(nlohmann::json message) -> ::HRESULT
    {
        if (m_webView.core20)
        {
            auto wideJson{glow::wstring(message.dump())};

            if (wideJson.empty()) { return S_OK; }

            else return m_webView.core20->PostWebMessageAsJson(wideJson.c_str());
        }

        else return S_OK;
    }

    auto
    move_focus(COREWEBVIEW2_MOVE_FOCUS_REASON reason = COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC)
        -> ::HRESULT
    {
        if (m_webView.controller4) { return m_webView.controller4->MoveFocus(reason); }

        else return S_OK;
    }

    auto zoom(double zoomFactor) -> ::HRESULT
    {
        if (m_webView.controller4) { return m_webView.controller4->put_ZoomFactor(zoomFactor); }

        else return S_OK;
    }

    auto visibile(bool visible) -> ::HRESULT
    {
        if (m_webView.controller4)
        {
            if (visible) { return m_webView.controller4->put_IsVisible(true); }

            else { return m_webView.controller4->put_IsVisible(false); }
        }

        else return S_OK;
    }

    auto devtools() -> ::HRESULT
    {
        if (m_webView.core20) { return m_webView.core20->OpenDevToolsWindow(); }

        else return S_OK;
    }

    auto version() -> std::string
    {
        wil::unique_cotaskmem_string buffer;
        GetAvailableCoreWebView2BrowserVersionString(nullptr, &buffer);

        return glow::string(buffer.get());
    }

    ::HWND m_parent{nullptr};
    WebView2 m_webView{};

  private:
    T& derived() { return static_cast<T&>(*this); }
    T const& derived() const { return static_cast<T const&>(*this); }
};
} // namespace glow
