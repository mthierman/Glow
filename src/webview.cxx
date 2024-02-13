// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "webview.hxx"

namespace glow
{
WebView::WebView(HWND parent, intptr_t id)
    : Window("WebView", id, WS_CHILD, 0, 0, 0, 0, 0, parent, reinterpret_cast<::HMENU>(id)),
      m_parent{parent}
{
    if (FAILED(create())) { throw std::runtime_error("WebView creation failure"); }
}

WebView::~WebView()
{
    m_core->remove_ContextMenuRequested(m_tokenContextMenuRequested);
    m_core->remove_SourceChanged(m_tokenSourceChanged);
    m_core->remove_NavigationStarting(m_tokenNavigationStarting);
    m_core->remove_NavigationCompleted(m_tokenNavigationCompleted);
    m_core->remove_WebMessageReceived(m_tokenWebMessageReceived);
    m_core->remove_DocumentTitleChanged(m_tokenDocumentTitleChanged);
    m_core->remove_FaviconChanged(m_tokenFaviconChanged);
    m_controller->remove_AcceleratorKeyPressed(m_tokenAcceleratorKeyPressed);
    m_controller->remove_ZoomFactorChanged(m_tokenZoomFactorChanged);
    m_controller->remove_GotFocus(m_tokenGotFocus);
    m_controller->remove_LostFocus(m_tokenLostFocus);
    m_controller->remove_MoveFocusRequested(m_tokenMoveFocusRequested);
}

auto WebView::create() -> ::HRESULT
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

auto WebView::create_controller(ICoreWebView2Environment* createdEnvironment) -> ::HRESULT
{
    return createdEnvironment->CreateCoreWebView2Controller(
        hwnd(),
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
            [=, this](::HRESULT errorCode, ICoreWebView2Controller* createdController) -> ::HRESULT
            {
                if (createdController)
                {
                    m_initController = createdController;
                    m_controller = m_initController.try_query<ICoreWebView2Controller4>();

                    if (!m_controller) return E_POINTER;

                    COREWEBVIEW2_COLOR bgColor{0, 0, 0, 0};
                    m_controller->put_DefaultBackgroundColor(bgColor);

                    ::SendMessageA(m_parent, WM_SIZE, 0, 0);
                    ::SendMessageA(hwnd(), WM_SIZE, 0, 0);

                    m_controller->get_CoreWebView2(m_initCore.put());
                    m_core = m_initCore.try_query<ICoreWebView2_21>();

                    if (!m_core) return E_POINTER;

                    m_core->get_Settings(m_initSettings.put());

                    m_settings = m_initSettings.try_query<ICoreWebView2Settings8>();

                    if (!m_settings) return E_POINTER;

                    put_settings();

                    add_context_menu_requested();
                    add_source_changed();
                    add_navigation_starting();
                    add_navigation_completed();
                    add_web_message_received();
                    add_accelerator_key_pressed();
                    add_favicon_changed();
                    add_document_title_changed();
                    add_zoom_factor_changed();
                    add_got_focus();
                    add_lost_focus();
                    add_move_focus_requested();

                    created();

                    return S_OK;
                }

                else { return errorCode; }
            })
            .Get());
}

auto WebView::created() -> ::HRESULT { return S_OK; }

auto WebView::put_settings() -> ::HRESULT
{
    m_settings->put_AreBrowserAcceleratorKeysEnabled(true);
    m_settings->put_AreDefaultContextMenusEnabled(true);
    m_settings->put_AreDefaultScriptDialogsEnabled(true);
    m_settings->put_AreDevToolsEnabled(true);
    m_settings->put_AreHostObjectsAllowed(true);
    m_settings->put_HiddenPdfToolbarItems(
        COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE);
    m_settings->put_IsBuiltInErrorPageEnabled(true);
    m_settings->put_IsGeneralAutofillEnabled(true);
    m_settings->put_IsPasswordAutosaveEnabled(true);
    m_settings->put_IsPinchZoomEnabled(true);
    m_settings->put_IsReputationCheckingRequired(true);
    m_settings->put_IsScriptEnabled(true);
    m_settings->put_IsStatusBarEnabled(true);
    m_settings->put_IsSwipeNavigationEnabled(true);
    m_settings->put_IsWebMessageEnabled(true);
    m_settings->put_IsZoomControlEnabled(true);

    return S_OK;
}

auto WebView::add_context_menu_requested() -> ::HRESULT
{
    return m_core->add_ContextMenuRequested(
        Microsoft::WRL::Callback<ICoreWebView2ContextMenuRequestedEventHandler>(
            [=, this](ICoreWebView2* sender,
                      ICoreWebView2ContextMenuRequestedEventArgs* args) -> ::HRESULT
            { return context_menu_requested_handler(sender, args); })
            .Get(),
        &m_tokenContextMenuRequested);
}

auto WebView::context_menu_requested_handler(ICoreWebView2* sender,
                                             ICoreWebView2ContextMenuRequestedEventArgs* args)
    -> ::HRESULT
{
    return S_OK;
}

auto WebView::add_source_changed() -> ::HRESULT
{
    return m_core->add_SourceChanged(
        Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>(
            [=, this](ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args) -> ::HRESULT
            { return source_changed_handler(sender, args); })
            .Get(),
        &m_tokenSourceChanged);
}

auto WebView::source_changed_handler(ICoreWebView2* sender,
                                     ICoreWebView2SourceChangedEventArgs* args) -> ::HRESULT
{
    return S_OK;
}

auto WebView::add_navigation_starting() -> ::HRESULT
{
    return m_core->add_NavigationStarting(
        Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
            [=, this](ICoreWebView2* sender,
                      ICoreWebView2NavigationStartingEventArgs* args) -> ::HRESULT
            { return navigation_starting_handler(sender, args); })
            .Get(),
        &m_tokenNavigationStarting);
}

auto WebView::navigation_starting_handler(ICoreWebView2* sender,
                                          ICoreWebView2NavigationStartingEventArgs* args)
    -> ::HRESULT
{
    return S_OK;
}

auto WebView::add_navigation_completed() -> ::HRESULT
{
    return m_core->add_NavigationCompleted(
        Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
            [=, this](ICoreWebView2* sender,
                      ICoreWebView2NavigationCompletedEventArgs* args) -> ::HRESULT
            { return navigation_completed_handler(sender, args); })
            .Get(),
        &m_tokenNavigationCompleted);
}

auto WebView::navigation_completed_handler(ICoreWebView2* sender,
                                           ICoreWebView2NavigationCompletedEventArgs* args)
    -> ::HRESULT
{
    return S_OK;
}

auto WebView::add_web_message_received() -> ::HRESULT
{
    return m_core->add_WebMessageReceived(
        Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
            [=, this](ICoreWebView2* sender,
                      ICoreWebView2WebMessageReceivedEventArgs* args) -> ::HRESULT
            { return web_message_received_handler(sender, args); })
            .Get(),
        &m_tokenWebMessageReceived);
}

auto WebView::web_message_received_handler(ICoreWebView2* sender,
                                           ICoreWebView2WebMessageReceivedEventArgs* args)
    -> ::HRESULT
{
    return S_OK;
}

auto WebView::add_document_title_changed() -> ::HRESULT
{
    return m_core->add_DocumentTitleChanged(
        Microsoft::WRL::Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
            [=, this](ICoreWebView2* sender, IUnknown* args) -> ::HRESULT
            { return document_title_changed_handler(sender, args); })
            .Get(),
        &m_tokenDocumentTitleChanged);
}

auto WebView::document_title_changed_handler(ICoreWebView2* sender, IUnknown* args) -> ::HRESULT
{
    return S_OK;
}

auto WebView::add_favicon_changed() -> ::HRESULT
{
    return m_core->add_FaviconChanged(
        Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
            [=, this](ICoreWebView2* sender, IUnknown* args) -> ::HRESULT
            { return favicon_changed_handler(sender, args); })
            .Get(),
        &m_tokenFaviconChanged);
}

auto WebView::favicon_changed_handler(ICoreWebView2* sender, IUnknown* args) -> ::HRESULT
{
    return S_OK;
}

auto WebView::add_accelerator_key_pressed() -> ::HRESULT
{
    return m_controller->add_AcceleratorKeyPressed(
        Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
            [=, this](ICoreWebView2Controller* sender,
                      ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> ::HRESULT
            { return accelerator_key_pressed_handler(sender, args); })
            .Get(),
        &m_tokenAcceleratorKeyPressed);
}

auto WebView::accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                              ICoreWebView2AcceleratorKeyPressedEventArgs* args)
    -> ::HRESULT
{
    return S_OK;
}

auto WebView::add_zoom_factor_changed() -> ::HRESULT
{
    return m_controller->add_ZoomFactorChanged(
        Microsoft::WRL::Callback<ICoreWebView2ZoomFactorChangedEventHandler>(
            [=, this](ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT
            { return zoom_factor_changed_handler(sender, args); })
            .Get(),
        &m_tokenZoomFactorChanged);
}

auto WebView::zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
    -> ::HRESULT
{
    return S_OK;
}

auto WebView::add_got_focus() -> ::HRESULT
{
    return m_controller->add_GotFocus(
        Microsoft::WRL::Callback<ICoreWebView2FocusChangedEventHandler>(
            [=, this](ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT
            { return got_focus_handler(sender, args); })
            .Get(),
        &m_tokenGotFocus);
}

auto WebView::got_focus_handler(ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT
{
    return S_OK;
}

auto WebView::add_lost_focus() -> ::HRESULT
{
    return m_controller->add_LostFocus(
        Microsoft::WRL::Callback<ICoreWebView2FocusChangedEventHandler>(
            [=, this](ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT
            { return lost_focus_handler(sender, args); })
            .Get(),
        &m_tokenLostFocus);
}

auto WebView::lost_focus_handler(ICoreWebView2Controller* sender, IUnknown* args) -> ::HRESULT
{
    return S_OK;
}

auto WebView::add_move_focus_requested() -> ::HRESULT
{
    return m_controller->add_MoveFocusRequested(
        Microsoft::WRL::Callback<ICoreWebView2MoveFocusRequestedEventHandler>(
            [=, this](ICoreWebView2Controller* sender,
                      ICoreWebView2MoveFocusRequestedEventArgs* args) -> ::HRESULT
            { return move_focus_requested_handler(sender, args); })
            .Get(),
        &m_tokenMoveFocusRequested);
}

auto WebView::move_focus_requested_handler(ICoreWebView2Controller* sender,
                                           ICoreWebView2MoveFocusRequestedEventArgs* args)
    -> ::HRESULT
{
    return S_OK;
}

auto WebView::navigate(std::string url) -> ::HRESULT
{
    if (m_core)
    {
        auto wideUrl{glow::wstring(url)};

        if (wideUrl.empty()) { return S_OK; }

        else return m_core->Navigate(wideUrl.c_str());
    }

    else return S_OK;
}

auto WebView::post_json(nlohmann::json message) -> ::HRESULT
{
    if (m_core)
    {
        auto wideJson{glow::wstring(message.dump())};

        if (wideJson.empty()) { return S_OK; }

        else return m_core->PostWebMessageAsJson(wideJson.c_str());
    }

    else return S_OK;
}

auto WebView::get_document_title() -> std::string
{
    if (m_core)
    {
        wil::unique_cotaskmem_string buffer;
        if (FAILED(m_core->get_DocumentTitle(&buffer))) { return {}; }

        return glow::string(buffer.get());
    }

    else return {};
}

auto WebView::get_favicon_url() -> std::string
{
    if (m_core)
    {
        wil::unique_cotaskmem_string buffer;
        if (FAILED(m_core->get_FaviconUri(&buffer))) { return {}; }

        return glow::string(buffer.get());
    }

    else return {};
}

auto WebView::devtools() -> ::HRESULT
{
    if (m_core) { return m_core->OpenDevToolsWindow(); }

    else return S_OK;
}

auto WebView::move_focus(COREWEBVIEW2_MOVE_FOCUS_REASON reason) -> ::HRESULT
{
    if (m_controller) { return m_controller->MoveFocus(reason); }

    else return S_OK;
}

auto WebView::zoom(double zoomFactor) -> ::HRESULT
{
    if (m_controller) { return m_controller->put_ZoomFactor(zoomFactor); }

    else return S_OK;
}

auto WebView::visible(bool visible) -> ::HRESULT
{
    if (m_controller)
    {
        if (visible) { return m_controller->put_IsVisible(true); }

        else { return m_controller->put_IsVisible(false); }
    }

    else return S_OK;
}

auto WebView::version() -> std::string
{
    wil::unique_cotaskmem_string buffer;
    GetAvailableCoreWebView2BrowserVersionString(nullptr, &buffer);

    return glow::string(buffer.get());
}

auto WebView::wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    switch (uMsg)
    {
    case WM_SIZE: return on_size(wParam, lParam);
    }

    return ::DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto WebView::on_size(::WPARAM wParam, ::LPARAM lParam) -> int
{
    if (m_controller)
    {
        position();
        m_controller->put_Bounds(m_client.rect);
    }

    return 0;
}
} // namespace glow
