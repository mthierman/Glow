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
WebView::WebView(::HWND parent, std::function<::HRESULT()> callback, size_t id)
    : Window("WebView", id, WS_CHILD, 0, 0, 0, 0, 0, parent, reinterpret_cast<::HMENU>(id)),
      m_callback{callback}
{}

WebView::~WebView()
{
    if (m_core)
    {
        m_core->remove_ContextMenuRequested(m_tokenContextMenuRequested);
        m_core->remove_SourceChanged(m_tokenSourceChanged);
        m_core->remove_NavigationStarting(m_tokenNavigationStarting);
        m_core->remove_NavigationCompleted(m_tokenNavigationCompleted);
        m_core->remove_WebMessageReceived(m_tokenWebMessageReceived);
        m_core->remove_DocumentTitleChanged(m_tokenDocumentTitleChanged);
        m_core->remove_FaviconChanged(m_tokenFaviconChanged);
    }

    if (m_controller)
    {
        m_controller->remove_AcceleratorKeyPressed(m_tokenAcceleratorKeyPressed);
        m_controller->remove_ZoomFactorChanged(m_tokenZoomFactorChanged);
        m_controller->remove_GotFocus(m_tokenGotFocus);
        m_controller->remove_LostFocus(m_tokenLostFocus);
        m_controller->remove_MoveFocusRequested(m_tokenMoveFocusRequested);
    }
}

auto WebView::create_environment(std::function<::HRESULT()> callback) -> ::HRESULT
{
    return CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [=, this](::HRESULT errorCode,
                      ICoreWebView2Environment* createdEnvironment) -> ::HRESULT
            {
                if (!createdEnvironment) { return errorCode; }

                return create_controller(createdEnvironment, callback);
            })
            .Get());
}

auto WebView::create_controller(ICoreWebView2Environment* createdEnvironment,
                                std::function<::HRESULT()> callback) -> ::HRESULT
{
    return createdEnvironment->CreateCoreWebView2Controller(
        m_hwnd.get(),
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
            [=, this](::HRESULT errorCode, ICoreWebView2Controller* createdController) -> ::HRESULT
            {
                if (createdController)
                {
                    m_initController = createdController;
                    m_controller = m_initController.try_query<ICoreWebView2Controller4>();

                    if (!m_controller) return E_POINTER;

                    m_controller->put_DefaultBackgroundColor(COREWEBVIEW2_COLOR{0, 0, 0, 0});

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

                    callback();

                    return S_OK;
                }

                else { return errorCode; }
            })
            .Get());
}

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
            {
                wil::unique_cotaskmem_string source;
                if (SUCCEEDED(sender->get_Source(&source)))
                {
                    m_source.assign(glow::string(source.get()));
                }

                return source_changed_handler(sender, args);
            })
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
            {
                wil::unique_cotaskmem_string buffer;

                if (SUCCEEDED(m_core->get_DocumentTitle(&buffer)))
                {
                    m_title = glow::string(buffer.get());
                }

                return document_title_changed_handler(sender, args);
            })
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
            {
                wil::unique_cotaskmem_string buffer;
                if (SUCCEEDED(m_core->get_FaviconUri(&buffer)))
                {
                    m_favicon.first.assign(glow::string(buffer.get()));
                }

                return favicon_changed_handler(sender, args);
            })
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
    if (!m_core) { return S_OK; }

    auto wideUrl{glow::wstring(url)};

    if (wideUrl.empty()) { return S_OK; }

    return m_core->Navigate(wideUrl.c_str());
}

auto WebView::post_json(nlohmann::json message) -> ::HRESULT
{
    if (!m_core) { return S_OK; }

    auto wideJson{glow::wstring(message.dump())};

    if (wideJson.empty()) { return S_OK; }

    return m_core->PostWebMessageAsJson(wideJson.c_str());
}

auto WebView::get_favicon(std::function<::HRESULT()> callback) -> ::HRESULT
{
    return m_core->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                              Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
                                  [=, this](::HRESULT errorCode, ::IStream* iconStream) -> ::HRESULT
                                  {
                                      if (FAILED(errorCode)) { return S_OK; }

                                      Gdiplus::Bitmap iconBitmap(iconStream);

                                      iconBitmap.GetHICON(&m_favicon.second);

                                      callback();

                                      return S_OK;
                                  })
                                  .Get());
}

auto WebView::open_dev_tools_window() -> ::HRESULT
{
    if (!m_core) { return S_OK; }

    return m_core->OpenDevToolsWindow();
}

auto WebView::move_focus(COREWEBVIEW2_MOVE_FOCUS_REASON reason) -> ::HRESULT
{
    if (!m_controller) { return S_OK; }

    return m_controller->MoveFocus(reason);
}

auto WebView::put_zoom_factor(double zoomFactor) -> ::HRESULT
{
    if (!m_controller) { return S_OK; }

    return m_controller->put_ZoomFactor(zoomFactor);
}

auto WebView::put_is_visible(bool visible) -> ::HRESULT
{
    if (!m_controller) { return S_OK; }

    if (visible) { return m_controller->put_IsVisible(true); }

    else { return m_controller->put_IsVisible(false); }
}

auto WebView::webview_version() -> std::expected<std::string, ::HRESULT>
{
    wil::unique_cotaskmem_string buffer;

    if (auto hr{GetAvailableCoreWebView2BrowserVersionString(nullptr, &buffer)}; FAILED(hr))
    {
        return std::unexpected(hr);
    }

    return glow::string(buffer.get());
}

auto WebView::WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    switch (uMsg)
    {
        case WM_SIZE: return on_size(wParam, lParam);
    }

    return ::DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto WebView::on_create(::WPARAM wParam, ::LPARAM lParam) -> int
{
    position();

    if (FAILED(create_environment(m_callback)))
    {
        throw std::runtime_error("WebView creation failure");
    }

    return 0;
}

auto WebView::on_size(::WPARAM wParam, ::LPARAM lParam) -> int
{
    if (!m_controller) { return 0; }

    position();
    m_controller->put_Bounds(m_client.rect);

    return 0;
}
} // namespace glow
