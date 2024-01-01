// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <gui/webview.hxx>

namespace glow::gui
{

WebView2::WebView2(HWND hwndParent, int64_t id, std::string url)
{
    m_hwndParent = hwndParent;
    m_id = id;
    m_url = url;

    WNDCLASSEXA wcex{sizeof(WNDCLASSEXA)};
    auto classInfo{
        GetClassInfoExA(GetModuleHandleA(nullptr), MAKEINTATOM(WebView2::m_atom), &wcex)};

    if (!classInfo)
    {
        OutputDebugStringA("Registering WebView2 class...");

        wcex.lpszClassName = "WebView2";
        wcex.lpszMenuName = 0;
        wcex.lpfnWndProc = WebView2::WndProc;
        wcex.style = 0;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(void*);
        wcex.hInstance = GetModuleHandleA(nullptr);
        wcex.hbrBackground = m_hbrBackground.get();
        wcex.hCursor = m_hCursor.get();
        wcex.hIcon = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();
        wcex.hIconSm = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();

        WebView2::m_atom = RegisterClassExA(&wcex);
    }

    CreateWindowExA(0, MAKEINTATOM(WebView2::m_atom), "WebView2", WS_CHILD, 0, 0, 0, 0,
                    m_hwndParent, std::bit_cast<HMENU>(m_id), GetModuleHandleA(nullptr), this);

    create_environment();
}

WebView2::~WebView2() {}

auto WebView2::show_normal() -> void { glow::gui::show_normal(m_hwnd.get()); }

auto WebView2::show() -> void { glow::gui::show(m_hwnd.get()); }

auto WebView2::hide() -> void { glow::gui::hide(m_hwnd.get()); }

auto CALLBACK WebView2::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto self{InstanceFromWndProc<WebView2>(hWnd, uMsg, lParam)};

    if (self)
    {
        switch (uMsg)
        {
        case WM_CLOSE: return self->on_close();
        case WM_SIZE: return self->on_size();
        }

        return self->handle_wnd_proc(hWnd, uMsg, wParam, lParam);
    }

    else return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto WebView2::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto WebView2::on_close() -> int
{
    m_hwnd.reset();

    return 0;
}

auto WebView2::on_size() -> int
{
    RECT rect{};
    GetClientRect(m_hwnd.get(), &rect);
    if (m_controller4) m_controller4->put_Bounds(rect);

    return 0;
}

auto WebView2::create_environment() -> void
{
    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [=, this](HRESULT errorCode, ICoreWebView2Environment* createdEnvironment) -> HRESULT
            {
                if (createdEnvironment) create_controller(createdEnvironment);

                return S_OK;
            })
            .Get());
}

auto WebView2::create_controller(ICoreWebView2Environment* environment) -> void
{
    environment->CreateCoreWebView2Controller(
        m_hwnd.get(),
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
            [=, this](HRESULT, ICoreWebView2Controller* controller) -> HRESULT
            {
                if (controller)
                {
                    m_controller = controller;
                    m_controller4 = m_controller.try_query<ICoreWebView2Controller4>();

                    COREWEBVIEW2_COLOR bgColor{0, 0, 0, 0};
                    m_controller4->put_DefaultBackgroundColor(bgColor);

                    m_controller->get_CoreWebView2(m_core.put());
                    m_core20 = m_core.try_query<ICoreWebView2_20>();

                    m_core20->get_Settings(m_settings.put());

                    m_settings8 = m_settings.try_query<ICoreWebView2Settings8>();

                    m_settings8->put_AreBrowserAcceleratorKeysEnabled(true);
                    m_settings8->put_AreDefaultContextMenusEnabled(true);
                    m_settings8->put_AreDefaultScriptDialogsEnabled(true);
                    m_settings8->put_AreDevToolsEnabled(true);
                    m_settings8->put_AreHostObjectsAllowed(true);
                    m_settings8->put_HiddenPdfToolbarItems(
                        COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE);
                    m_settings8->put_IsBuiltInErrorPageEnabled(true);
                    m_settings8->put_IsGeneralAutofillEnabled(true);
                    m_settings8->put_IsPasswordAutosaveEnabled(true);
                    m_settings8->put_IsPinchZoomEnabled(true);
                    m_settings8->put_IsReputationCheckingRequired(true);
                    m_settings8->put_IsScriptEnabled(true);
                    m_settings8->put_IsStatusBarEnabled(true);
                    m_settings8->put_IsSwipeNavigationEnabled(true);
                    m_settings8->put_IsWebMessageEnabled(true);
                    m_settings8->put_IsZoomControlEnabled(true);

                    m_core20->Navigate(glow::text::widen(m_url).c_str());

                    if (!m_initialized)
                    {
                        m_initialized = true;
                        initialized();
                        on_size();
                    }

                    source_changed();
                    navigation_completed();
                    web_message_received();
                    accelerator_key_pressed();
                    favicon_changed();
                    document_title_changed();
                }

                return S_OK;
            })
            .Get());
}

auto WebView2::navigate(std::string url) -> void
{
    auto wideUrl{glow::text::widen(url)};
    if (m_core20) m_core20->Navigate(wideUrl.c_str());
}

auto WebView2::post_json(const json jsonMessage) -> void
{
    auto wideUrl{glow::text::widen(jsonMessage)};
    if (m_core20) m_core20->PostWebMessageAsJson(wideUrl.c_str());
}

auto WebView2::source_changed() -> void
{
    EventRegistrationToken token;

    m_core20->add_SourceChanged(
        Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>(
            [=, this](ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
            {
                // source_changed_handler();

                return S_OK;
            })
            .Get(),
        &token);
}

auto WebView2::navigation_completed() -> void
{
    EventRegistrationToken token;

    m_core20->add_NavigationCompleted(
        Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
            [=, this](ICoreWebView2* sender,
                      ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
            {
                // navigation_completed_handler();

                return S_OK;
            })
            .Get(),
        &token);
}

auto WebView2::web_message_received() -> void
{
    EventRegistrationToken token;

    m_core20->add_WebMessageReceived(
        Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
            [=, this](ICoreWebView2* sender,
                      ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
            {
                // web_message_received_handler();

                return S_OK;
            })
            .Get(),
        &token);
}

auto WebView2::accelerator_key_pressed() -> void
{
    EventRegistrationToken token;

    m_controller4->add_AcceleratorKeyPressed(
        Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
            [=, this](ICoreWebView2Controller* sender,
                      ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT
            {
                // accelerator_key_pressed_handler(args);

                return S_OK;
            })
            .Get(),
        &token);
}

auto WebView2::favicon_changed() -> void
{
    EventRegistrationToken token;

    m_core20->add_FaviconChanged(Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                                     [=, this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                     {
                                         // favicon_changed_handler();

                                         return S_OK;
                                     })
                                     .Get(),
                                 &token);
}

auto WebView2::document_title_changed() -> void
{
    EventRegistrationToken token;

    m_core20->add_DocumentTitleChanged(
        Microsoft::WRL::Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
            [=, this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
            {
                // document_title_changed_handler();

                return S_OK;
            })
            .Get(),
        &token);
}

} // namespace glow::gui
