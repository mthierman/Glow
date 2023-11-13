#pragma once

#include <Windows.h>
#include <wrl.h>
#include <windows.foundation.h>

#include "WebView2.h"

#include <string>

#include "../gui/gui.hxx"
#include "../text/text.hxx"

namespace glow::gui
{
struct WebView
{
  public:
    WebView(std::string, HWND, int);
    ~WebView();

    static LRESULT CALLBACK WndProcCallback(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
    virtual int OnClose(HWND);
    virtual int OnWindowPosChanged(HWND);

    HWND parentHwnd{nullptr};
    HWND webviewHwnd{nullptr};
    UINT_PTR id{0};
    bool initialized{false};
    winrt::com_ptr<ICoreWebView2Controller> controller{nullptr};
    winrt::com_ptr<ICoreWebView2Controller4> controller4{nullptr};
    winrt::com_ptr<ICoreWebView2> core{nullptr};
    winrt::com_ptr<ICoreWebView2_19> core19{nullptr};
    winrt::com_ptr<ICoreWebView2Settings> settings{nullptr};
};

WebView::WebView(std::string n, HWND h, int i) : parentHwnd(h), id(i)
{
    auto brush{reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH))};

    auto cursor{
        reinterpret_cast<HCURSOR>(::LoadImage(nullptr, reinterpret_cast<LPCSTR>(IDC_ARROW),
                                              IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    auto icon{
        reinterpret_cast<HICON>(::LoadImage(nullptr, reinterpret_cast<LPCSTR>(IDI_APPLICATION),
                                            IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

    auto name{glow::text::randomize(n)};

    WNDCLASSEX wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = name.c_str();
    wcex.lpszMenuName = name.c_str();
    wcex.lpfnWndProc = WebView::WndProcCallback;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = ::GetModuleHandle(nullptr);
    wcex.hbrBackground = brush;
    wcex.hCursor = cursor;
    wcex.hIcon = icon;
    wcex.hIconSm = icon;

    ::RegisterClassEx(&wcex);

    ::CreateWindowEx(0, name.c_str(), name.c_str(), WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT,
                     CW_USEDEFAULT, CW_USEDEFAULT, parentHwnd, reinterpret_cast<HMENU>(id),
                     ::GetModuleHandle(nullptr), this);

    window_cloak(webviewHwnd);
    window_mica(webviewHwnd);

    ::ShowWindow(webviewHwnd, SW_SHOW);

    winrt::check_hresult(CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [=, this](HRESULT, ICoreWebView2Environment* e) -> HRESULT
            {
                if (e)
                    winrt::check_hresult(e->CreateCoreWebView2Controller(
                        webviewHwnd,
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
                                    GetClientRect(webviewHwnd, &bounds);
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
                                            [=, this](ICoreWebView2* /*webview*/,
                                                      ICoreWebView2NavigationCompletedEventArgs*
                                                      /*args*/) -> HRESULT
                                            {
                                                if (!initialized)
                                                {
                                                    window_uncloak(webviewHwnd);
                                                    initialized = true;
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
};

WebView::~WebView() {}

LRESULT CALLBACK WebView::WndProcCallback(HWND h, UINT m, WPARAM w, LPARAM l)
{
    WebView* webview = InstanceFromWndProc<WebView, &WebView::webviewHwnd>(h, m, l);

    if (webview)
    {
        switch (m)
        {
        case WM_CLOSE: return webview->OnClose(h);
        case WM_WINDOWPOSCHANGED: return webview->OnWindowPosChanged(h);
        }

        return webview->WndProc(h, m, w, l);
    }

    return ::DefWindowProc(h, m, w, l);
}

LRESULT WebView::WndProc(HWND h, UINT m, WPARAM w, LPARAM l) { return ::DefWindowProc(h, m, w, l); }

int WebView::OnClose(HWND h)
{
    ::DestroyWindow(h);

    return 0;
}

int WebView::OnWindowPosChanged(HWND h)
{
    RECT r;
    ::GetClientRect(h, &r);

    if (controller4) controller4->put_Bounds(r);

    return 0;
}
} // namespace glow::gui
