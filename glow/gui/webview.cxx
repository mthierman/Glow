// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <gui/webview.hxx>

//==============================================================================
namespace glow::gui
{

//==============================================================================
WebView::WebView(std::string name, HWND parentHwnd, int id)
    : m_name(name), m_class(glow::text::randomize(name)), m_hwndParent(parentHwnd), m_id(id)
{
    ::SetEnvironmentVariable("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    ::SetEnvironmentVariable("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                             "--allow-file-access-from-files");

    m_classAtom = register_window();
    create_window();
    show_window_default();

    window_cloak(m_hwnd.get());
    create_environment();
}

//==============================================================================
WebView::~WebView() {}

//==============================================================================
auto WebView::register_window() -> ATOM
{
    WNDCLASSEX wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = m_class.c_str();
    wcex.lpszMenuName = m_class.c_str();
    wcex.lpfnWndProc = WebView::wnd_proc;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = ::GetModuleHandle(nullptr);
    wcex.hbrBackground = m_background;
    wcex.hCursor = m_cursor;
    wcex.hIcon = m_icon ? m_icon : m_defaultIcon;
    wcex.hIconSm = m_icon ? m_icon : m_defaultIcon;

    return ::RegisterClassEx(&wcex);
}

//==============================================================================
auto WebView::create_window() -> void
{
    ::CreateWindowEx(0, MAKEINTATOM(m_classAtom), m_name.c_str(), WS_CHILD, CW_USEDEFAULT,
                     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_hwndParent.get(),
                     reinterpret_cast<HMENU>(m_id), ::GetModuleHandle(nullptr), this);
}

//==============================================================================
auto WebView::show_window_default() -> void { ::ShowWindow(m_hwnd.get(), SW_SHOWDEFAULT); }

//==============================================================================
auto WebView::show_window() -> void { ::ShowWindow(m_hwnd.get(), SW_SHOW); }

//==============================================================================
auto WebView::hide_window() -> void { ::ShowWindow(m_hwnd.get(), SW_HIDE); }

//==============================================================================
auto WebView::create_environment() -> void
{
    winrt::check_hresult(CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [=, this](HRESULT, ICoreWebView2Environment* environment) -> HRESULT
            {
                if (environment) create_controller(environment);

                return S_OK;
            })
            .Get()));
}

//==============================================================================
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
                    m_core19 = m_core.as<ICoreWebView2_19>();

                    winrt::check_hresult(m_core19->get_Settings(m_settings.put()));

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

                    m_core19->Navigate(L"https://www.google.com/");

                    navigation_completed();
                    accelerator_key_pressed();
                }

                return S_OK;
            })
            .Get()));
}

//==============================================================================
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

    else return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//==============================================================================
auto WebView::handle_message(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return ::DefWindowProc(m_hwnd.get(), uMsg, wParam, lParam);
}

//==============================================================================
auto WebView::on_window_pos_changed() -> int
{
    if (m_controller4)
    {
        RECT rect{};
        ::GetClientRect(m_hwnd.get(), &rect);
        m_controller4->put_Bounds(rect);
    }

    return 0;
}

//==============================================================================
auto WebView::navigation_completed() -> void
{
    EventRegistrationToken navigationCompletedToken;

    winrt::check_hresult(m_core19->add_NavigationCompleted(
        Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
            [=, this](ICoreWebView2* webView,
                      ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
            {
                if (!m_initialized)
                {
                    window_uncloak(m_hwnd.get());
                    // ::SendMessage(m_hwndParent.get(), WM_NOTIFY, 0, 0);
                    ::SendMessage(m_hwndParent.get(), WM_WINDOWPOSCHANGED, 0, 0);
                    m_initialized = true;
                }

                return S_OK;
            })
            .Get(),
        &navigationCompletedToken));
}

//==============================================================================
auto WebView::web_message_received() -> void
{
    EventRegistrationToken webMessageReceivedToken;

    winrt::check_hresult(m_core19->add_WebMessageReceived(
        Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
            [=, this](ICoreWebView2* webView,
                      ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT { return S_OK; })
            .Get(),
        &webMessageReceivedToken));
}

//==============================================================================
auto WebView::accelerator_key_pressed() -> void
{
    EventRegistrationToken acceleratorKeyPressedToken;

    winrt::check_hresult(m_controller4->add_AcceleratorKeyPressed(
        Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
            [=, this](ICoreWebView2Controller* sender,
                      ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT
            { return S_OK; })
            .Get(),
        &acceleratorKeyPressedToken));
}

//==============================================================================
} // namespace glow::gui
