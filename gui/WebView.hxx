#pragma once

#include <Windows.h>
#include <wrl.h>
#include <windows.foundation.h>

#include <string>

#include "WebView2.h"

#include "../include/Helpers.hxx"

namespace glow::gui
{
struct WebView
{
    WebView(std::string, HWND, int);
    ~WebView();

    winrt::com_ptr<ICoreWebView2Controller> controller;
    winrt::com_ptr<ICoreWebView2Controller4> controller4;
    winrt::com_ptr<ICoreWebView2> core;
    winrt::com_ptr<ICoreWebView2_19> core19;
    winrt::com_ptr<ICoreWebView2Settings> settings;

  private:
    HWND parentHwnd;
    int id;
    HWND webviewHwnd;
    static LRESULT CALLBACK WndProcCallback(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    virtual int OnClose(HWND);
};

WebView::WebView(std::string n, HWND h, int i) : parentHwnd(h), id(i)
{
    auto b{reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH))};
    auto c{reinterpret_cast<HCURSOR>(::LoadImageW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW),
                                                  IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    auto i{reinterpret_cast<HICON>(::LoadImageW(nullptr, reinterpret_cast<LPCWSTR>(IDI_APPLICATION),
                                                IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

    auto className{glow::randomize(L"WebView")};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = className.c_str();
    wcex.lpszMenuName = className.c_str();
    wcex.lpfnWndProc = App::WndProcCallback;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = ::GetModuleHandleW(nullptr);
    wcex.hbrBackground = b;
    wcex.hCursor = c;
    wcex.hIcon = i;
    wcex.hIconSm = i;

    ::RegisterClassExW(&wcex);

    ::CreateWindowExW(0, className.c_str(), widen(n).c_str(), WS_CHILD, CW_USEDEFAULT,
                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parentHwnd,
                      reinterpret_cast<HMENU>(id), ::GetModuleHandleW(nullptr), this);

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
                                    // ShowWindow(this->webviewHwnd, SW_HIDE);
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

                                    EventRegistrationToken token;
                                    core19->add_NavigationCompleted(
                                        Microsoft::WRL::Callback<
                                            ICoreWebView2NavigationCompletedEventHandler>(
                                            [=, this](
                                                ICoreWebView2* /*webview*/,
                                                ICoreWebView2NavigationCompletedEventArgs* /*args*/)
                                                -> HRESULT
                                            {
                                                // if (!initialized)
                                                // {
                                                //     std::cout << "TEST" << std::endl;
                                                //     ShowWindow(this->m_hWnd, SW_SHOW);
                                                //     initialized = true;
                                                // }

                                                return S_OK;
                                            })
                                            .Get(),
                                        &token);
                                }

                                return S_OK;
                            })
                            .Get()));

                return S_OK;
            })
            .Get()));

    ShowWindow(webviewHwnd, SW_SHOWDEFAULT);
};

WebView::~WebView() {}

LRESULT CALLBACK WebView::WndProcCallback(HWND h, UINT m, WPARAM w, LPARAM l)
{
    WebView* webview = InstanceFromWndProc<WebView, &WebView::webviewHwnd>(h, m, l);

    if (webview)
    {
        switch (m)
        {
        case WM_CLOSE:
            return webview->OnClose(h);
        case WM_DESTROY:
            return ::DefWindowProcW(h, m, w, l);
        }

        return webview->WndProc(h, m, w, l);
    }

    return ::DefWindowProcW(h, m, w, l);
}

LRESULT App::WndProc(HWND h, UINT m, WPARAM w, LPARAM l) { return ::DefWindowProcW(h, m, w, l); }

int App::OnClose(HWND h)
{
    ::DestroyWindow(h);

    return 0;
}
} // namespace glow::gui
