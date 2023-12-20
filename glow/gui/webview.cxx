// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <gui/webview_window.hxx>

namespace glow::gui
{

WebViewWindow::WebViewWindow(std::string name, HWND parentHwnd, int64_t id)
    : m_class{glow::text::randomize(name.data())}, m_hwndParent{parentHwnd}, m_id{id}
{
    m_title = name;
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    m_atom = register_class();
    create_window();
    show_normal();

    create_environment();
}

auto WebViewWindow::register_class() -> ATOM
{
    wcex.lpszClassName = m_class.c_str();
    wcex.lpszMenuName = m_class.c_str();
    wcex.lpfnWndProc = Window::WndProc;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(void*);
    wcex.hInstance = GetModuleHandleA(nullptr);
    wcex.hbrBackground = m_lightHbrBackground.get();
    wcex.hCursor = m_hCursor.get();
    wcex.hIcon = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();
    wcex.hIconSm = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();

    return RegisterClassExA(&wcex);
}

auto WebViewWindow::create_window() -> HWND
{
    return CreateWindowExA(0, MAKEINTATOM(m_atom), m_title.c_str(), WS_CHILD, CW_USEDEFAULT,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_hwndParent.get(),
                           std::bit_cast<HMENU>(m_id), GetModuleHandleA(nullptr), this);
}

auto WebViewWindow::create_environment() -> void
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

auto WebViewWindow::create_controller(ICoreWebView2Environment* environment) -> void
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

                    // navigation_completed();
                    // accelerator_key_pressed();
                    // favicon_changed();
                    // document_title_changed();
                }

                return S_OK;
            })
            .Get()));
}

} // namespace glow::gui
