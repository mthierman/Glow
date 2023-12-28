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

WebView2::WebView2(HWND parentHwnd, int64_t id)
{
    m_hwndParent.reset(parentHwnd);
    m_id = id;
    create();
}

WebView2::~WebView2() {}

auto WebView2::create() -> void
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

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

    CreateWindowExA(0, MAKEINTATOM(WebView2::m_atom), "WebView2", WS_CHILD, CW_USEDEFAULT,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_hwndParent.get(),
                    std::bit_cast<HMENU>(m_id), GetModuleHandleA(nullptr), this);

    glow::gui::show_normal(m_hwnd.get());
    // glow::gui::window_cloak(m_hwnd.get());

    create_environment();
}

auto WebView2::create_environment() -> void
{
    winrt::check_hresult(CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [=, this](HRESULT errorCode, ICoreWebView2Environment* createdEnvironment) -> HRESULT
            {
                if (createdEnvironment) create_controller(createdEnvironment);

                return S_OK;
            })
            .Get()));
}

auto WebView2::create_controller(ICoreWebView2Environment* environment) -> void
{
    winrt::check_hresult(environment->CreateCoreWebView2Controller(
        m_hwnd.get(),
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
            [=, this](HRESULT, ICoreWebView2Controller* controller) -> HRESULT
            {
                if (controller)
                {
                    m_controller.attach(controller);
                    m_controller4 = m_controller.as<ICoreWebView2Controller4>();

                    COREWEBVIEW2_COLOR bgColor{0, 0, 0, 0};
                    m_controller4->put_DefaultBackgroundColor(bgColor);

                    winrt::check_hresult(m_controller->get_CoreWebView2(m_core.put()));
                    m_core20 = m_core.as<ICoreWebView2_20>();

                    winrt::check_hresult(m_core20->get_Settings(m_settings.put()));

                    m_settings8 = m_settings.as<ICoreWebView2Settings8>();

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

                    m_core20->Navigate(L"https://www.google.com/");

                    navigation_completed();
                    accelerator_key_pressed();
                    favicon_changed();
                    document_title_changed();
                    frame_navigation_starting();
                }

                return S_OK;
            })
            .Get()));
}

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

        return self->handle_message(hWnd, uMsg, wParam, lParam);
    }

    else return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto WebView2::handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
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
    if (m_controller4) { m_controller4->put_Bounds(rect); }

    return 0;
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

    winrt::check_hresult(m_core20->add_SourceChanged(
        Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>(
            [=, this](ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
            {
                // source_changed_handler();

                return S_OK;
            })
            .Get(),
        &token));
}

auto WebView2::navigation_completed() -> void
{
    EventRegistrationToken token;

    winrt::check_hresult(m_core20->add_NavigationCompleted(
        Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
            [=, this](ICoreWebView2* sender,
                      ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
            {
                initialize();

                return S_OK;
            })
            .Get(),
        &token));
}

auto WebView2::web_message_received() -> void
{
    EventRegistrationToken token;

    winrt::check_hresult(m_core20->add_WebMessageReceived(
        Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
            [=, this](ICoreWebView2* sender,
                      ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
            {
                // web_message_received_handler();

                return S_OK;
            })
            .Get(),
        &token));
}

auto WebView2::accelerator_key_pressed() -> void
{
    EventRegistrationToken token;

    winrt::check_hresult(m_controller4->add_AcceleratorKeyPressed(
        Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
            [=, this](ICoreWebView2Controller* sender,
                      ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT
            {
                // accelerator_key_pressed_handler(args);

                return S_OK;
            })
            .Get(),
        &token));
}

auto WebView2::favicon_changed() -> void
{
    EventRegistrationToken token;

    winrt::check_hresult(m_core20->add_FaviconChanged(
        Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
            [=, this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
            {
                // favicon_changed_handler();

                return S_OK;
            })
            .Get(),
        &token));
}

auto WebView2::document_title_changed() -> void
{
    EventRegistrationToken token;

    winrt::check_hresult(m_core20->add_DocumentTitleChanged(
        Microsoft::WRL::Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
            [=, this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
            {
                // document_title_changed_handler();

                return S_OK;
            })
            .Get(),
        &token));
}

auto WebView2::frame_navigation_starting() -> void
{
    EventRegistrationToken token;

    winrt::check_hresult(m_core20->add_FrameNavigationStarting(
        Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
            [=, this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
            {
                winrt::com_ptr<ICoreWebView2NavigationStartingEventArgs3> navigationStartArgs;
                // document_title_changed_handler();
                args->QueryInterface(IID_PPV_ARGS(&navigationStartArgs));
                navigationStartArgs->put_AdditionalAllowedFrameAncestors(L"*");
                return S_OK;
            })
            .Get(),
        &token));
}

auto WebView2::initialize() -> void
{
    if (!m_initialized)
    {
        m_initialized = true;
        SendMessageA(m_hwndParent.get(), WM_SIZE, 0, 0);
        // glow::gui::window_uncloak(m_hwnd.get());
    }
}

} // namespace glow::gui
