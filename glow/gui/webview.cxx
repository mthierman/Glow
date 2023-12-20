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
}

WebView2::WebView2(std::string title, HWND parentHwnd, int64_t id) : WebView2(parentHwnd, id)
{
    m_title = title;
}

auto WebView2::create() -> void
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    create_window();
    glow::gui::window_cloak(m_hwnd.get());
    show_normal();
    glow::gui::window_uncloak(m_hwnd.get());
}

auto WebView2::register_class() -> void
{
    wcex.lpszClassName = "WebView2";
    wcex.lpszMenuName = 0;
    wcex.lpfnWndProc = WebView2::WndProc;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(void*);
    wcex.hInstance = GetModuleHandleA(nullptr);
    wcex.hbrBackground = m_lightHbrBackground.get();
    wcex.hCursor = m_hCursor.get();
    wcex.hIcon = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();
    wcex.hIconSm = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();

    RegisterClassExA(&wcex);
}

auto WebView2::create_window() -> void
{
    auto classInfo{GetClassInfoExA(GetModuleHandleA(nullptr), "WebView2", &wcex)};
    if (!classInfo)
    {
        OutputDebugStringA("Registering WebView2 class...");
        register_class();
    }

    CreateWindowExA(0, "WebView2", "WebView2", WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT,
                    CW_USEDEFAULT, CW_USEDEFAULT, m_hwndParent.get(), std::bit_cast<HMENU>(m_id),
                    GetModuleHandleA(nullptr), this);
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
