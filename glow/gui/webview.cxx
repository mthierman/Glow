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
    : m_class(glow::text::randomize(name)), m_hwndParent(parentHwnd), m_id(id)
{
    register_window();
    create_window();
    show_window_default();
    window_cloak(m_hwnd.get());

    winrt::check_hresult(CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [=, this](HRESULT, ICoreWebView2Environment* e) -> HRESULT
            {
                if (e)
                    winrt::check_hresult(e->CreateCoreWebView2Controller(
                        m_hwnd.get(),
                        Microsoft::WRL::Callback<
                            ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                            [=, this](HRESULT, ICoreWebView2Controller* c) -> HRESULT
                            {
                                if (c)
                                {
                                    controller.attach(c);
                                    controller4 = controller.as<ICoreWebView2Controller4>();

                                    COREWEBVIEW2_COLOR bgColor{0, 0, 0, 0};
                                    controller4->put_DefaultBackgroundColor(bgColor);

                                    RECT bounds{0, 0, 0, 0};
                                    GetClientRect(m_hwnd.get(), &bounds);
                                    controller4->put_Bounds(bounds);

                                    winrt::check_hresult(c->get_CoreWebView2(core.put()));

                                    core19 = core.as<ICoreWebView2_19>();

                                    winrt::check_hresult(core19->get_Settings(settings.put()));

                                    settings->put_AreDefaultContextMenusEnabled(true);
                                    settings->put_AreDefaultScriptDialogsEnabled(true);
                                    settings->put_AreHostObjectsAllowed(true);
                                    settings->put_IsBuiltInErrorPageEnabled(true);
                                    settings->put_IsScriptEnabled(true);
                                    settings->put_IsStatusBarEnabled(false);
                                    settings->put_IsWebMessageEnabled(true);
                                    settings->put_IsZoomControlEnabled(false);
                                    settings->put_AreDevToolsEnabled(true);

                                    core19->Navigate(L"https://www.google.com/");

                                    EventRegistrationToken navigationCompletedToken;
                                    core19->add_NavigationCompleted(
                                        Microsoft::WRL::Callback<
                                            ICoreWebView2NavigationCompletedEventHandler>(
                                            [=,
                                             this](ICoreWebView2* webView,
                                                   ICoreWebView2NavigationCompletedEventArgs* args)
                                                -> HRESULT
                                            {
                                                if (!m_initialized)
                                                {
                                                    window_uncloak(m_hwnd.get());
                                                    ::SendMessage(m_hwndParent.get(), WM_NOTIFY, 0,
                                                                  0);
                                                    m_initialized = true;
                                                }

                                                return S_OK;
                                            })
                                            .Get(),
                                        &navigationCompletedToken);
                                }

                                return S_OK;
                            })
                            .Get()));

                return S_OK;
            })
            .Get()));
}

//==============================================================================
WebView::~WebView() {}

//==============================================================================
auto WebView::register_window() -> void
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

    ::RegisterClassEx(&wcex);
}

//==============================================================================
auto WebView::create_window() -> void
{
    ::CreateWindowEx(0, m_class.c_str(), m_class.c_str(), WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT,
                     CW_USEDEFAULT, CW_USEDEFAULT, m_hwndParent.get(),
                     reinterpret_cast<HMENU>(m_id), ::GetModuleHandle(nullptr), this);
}

//==============================================================================
auto WebView::show_window_default() -> void { ::ShowWindow(m_hwnd.get(), SW_SHOWDEFAULT); }

//==============================================================================
auto WebView::show_window() -> void { ::ShowWindow(m_hwnd.get(), SW_SHOW); }

//==============================================================================
auto WebView::hide_window() -> void { ::ShowWindow(m_hwnd.get(), SW_HIDE); }

//==============================================================================
auto CALLBACK WebView::wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    observer_ptr<WebView> self{InstanceFromWndProc<WebView>(hwnd, uMsg, lParam)};

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
    if (controller4)
    {
        RECT rect{};
        ::GetClientRect(m_hwnd.get(), &rect);
        controller4->put_Bounds(rect);
    }

    return 0;
}

//==============================================================================
} // namespace glow::gui
