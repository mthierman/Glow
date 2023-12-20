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

WebView::WebView(std::string_view name, HWND parentHwnd, int64_t id)
    : m_name(name), m_class(glow::text::randomize(name.data())), m_hwndParent(parentHwnd), m_id(id)
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    m_classAtom = register_window();
    create_window();
    show_window_default();

    window_cloak(m_hwnd.get());
    create_environment();
}

WebView::~WebView() {}

auto WebView::register_window() -> ATOM
{
    WNDCLASSEX wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = m_class.c_str();
    wcex.lpszMenuName = m_class.c_str();
    wcex.lpfnWndProc = WebView::wnd_proc;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(void*);
    wcex.hInstance = GetModuleHandleA(nullptr);
    wcex.hbrBackground = m_background;
    wcex.hCursor = m_cursor;
    wcex.hIcon = m_icon.get() ? m_icon.get() : m_defaultIcon;
    wcex.hIconSm = m_icon.get() ? m_icon.get() : m_defaultIcon;

    return RegisterClassExA(&wcex);
}

auto WebView::create_window() -> void
{
    CreateWindowExA(0, MAKEINTATOM(m_classAtom), m_name.c_str(), WS_CHILD, CW_USEDEFAULT,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_hwndParent.get(),
                    std::bit_cast<HMENU>(m_id), GetModuleHandleA(nullptr), this);
}

auto WebView::show_window_default() -> void { ShowWindow(m_hwnd.get(), SW_SHOWDEFAULT); }

auto WebView::show_window() -> void { ShowWindow(m_hwnd.get(), SW_SHOW); }

auto WebView::hide_window() -> void { ShowWindow(m_hwnd.get(), SW_HIDE); }

auto WebView::set_border(bool enabled) -> void
{
    auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_STYLE)};

    SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE,
                      enabled ? (style | WS_BORDER) : (style & ~WS_BORDER));
    SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto WebView::navigate(std::string_view url) -> void
{
    auto wideUrl{glow::text::widen(url.data())};
    if (m_core20) m_core20->Navigate(wideUrl.c_str());
}

auto WebView::post_json(const json jsonMessage) -> void
{
    auto wideUrl{glow::text::widen(jsonMessage)};
    if (m_core20) m_core20->PostWebMessageAsJson(wideUrl.c_str());
}

auto CALLBACK WebView::wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto self{InstanceFromWndProc<WebView>(hwnd, uMsg, lParam)};

    if (self)
    {
        switch (uMsg)
        {
        case WM_WINDOWPOSCHANGED: return self->on_window_pos_changed();
        }

        return self->handle_message(uMsg, wParam, lParam);
    }

    else return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

auto WebView::on_window_pos_changed() -> int
{
    RECT rect{};
    GetClientRect(m_hwnd.get(), &rect);
    if (m_controller4) { m_controller4->put_Bounds(rect); }

    return 0;
}

auto WebView::handle_message(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto WebView::create_environment() -> void
{
    // auto options{Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>()};
    // auto options6{Microsoft::WRL::ComPtr<ICoreWebView2EnvironmentOptions6>()};
    // options.As(&options6);
    // std::wstring browserArgsBuffer = L"--allow-file-access-from-files";
    // auto browserArgs = browserArgsBuffer.data();
    // options->get_AdditionalBrowserArguments(&browserArgs);

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

auto WebView::create_controller(ICoreWebView2Environment* environment) -> void
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
                }

                return S_OK;
            })
            .Get()));
}

auto WebView::source_changed() -> void
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

auto WebView::navigation_completed() -> void
{
    EventRegistrationToken token;

    winrt::check_hresult(m_core20->add_NavigationCompleted(
        Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
            [=, this](ICoreWebView2* sender,
                      ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
            {
                initialized();

                return S_OK;
            })
            .Get(),
        &token));
}

auto WebView::web_message_received() -> void
{
    EventRegistrationToken token;

    winrt::check_hresult(m_core20->add_WebMessageReceived(
        Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
            [=, this](ICoreWebView2* sender,
                      ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
            {
                web_message_received_handler();

                return S_OK;
            })
            .Get(),
        &token));
}

auto WebView::accelerator_key_pressed() -> void
{
    EventRegistrationToken token;

    winrt::check_hresult(m_controller4->add_AcceleratorKeyPressed(
        Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
            [=, this](ICoreWebView2Controller* sender,
                      ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT
            {
                accelerator_key_pressed_handler(args);

                return S_OK;
            })
            .Get(),
        &token));
}

auto WebView::favicon_changed() -> void
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

auto WebView::document_title_changed() -> void
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

auto WebView::initialized() -> void
{
    if (!m_initialized)
    {
        window_uncloak(m_hwnd.get());
        SendMessageA(m_hwndParent.get(), WM_NOTIFY, 0, 0);
        m_initialized = true;
    }
}

} // namespace glow::gui