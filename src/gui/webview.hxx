#pragma once

#include <Windows.h>
#include <string>
#include <filesystem>
#include "winrt/Windows.Foundation.h"
// #include "winrt/Microsoft.Web.WebView2.Core.h"
#include <WebView2.h>
#include <wrl.h>
#include "../helpers/helpers.hxx"

namespace glow
{
class WebView
{
  public:
    WebView(std::string, HWND);
    ~WebView();

  private:
    static __int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
    void create_webview(HWND);
    void create_controller(HWND, ICoreWebView2Environment*);
    bool get_core(ICoreWebView2Controller*);
    bool get_settings(ICoreWebView2*);

    HWND m_hwnd;

    winrt::com_ptr<ICoreWebView2Controller> controller;
    winrt::com_ptr<ICoreWebView2Controller4> controller4;
    winrt::com_ptr<ICoreWebView2> core;
    winrt::com_ptr<ICoreWebView2_19> core19;
    winrt::com_ptr<ICoreWebView2Settings> settings;
};

WebView::WebView(std::string name, HWND parentHwnd)
{
    auto r{glow::win32::randomize(name)};
    auto wideRandomName{glow::win32::to_wstring(r)};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = wideRandomName.c_str();
    wcex.lpszMenuName = wideRandomName.c_str();
    wcex.lpfnWndProc = WebView::WndProc;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandleW(nullptr);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.hCursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wcex.hIcon = (HICON)LoadImageW(nullptr, (LPCWSTR)IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                   LR_SHARED | LR_DEFAULTSIZE);
    wcex.hIconSm = (HICON)LoadImageW(nullptr, (LPCWSTR)IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                     LR_SHARED | LR_DEFAULTSIZE);

    auto atom{RegisterClassExW(&wcex)};

    if (atom == 0)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    CreateWindowExW(WS_EX_TRANSPARENT, wideRandomName.c_str(), wideRandomName.c_str(), WS_CHILD,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parentHwnd, nullptr,
                    GetModuleHandleW(nullptr), this);

    if (!m_hwnd)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    ShowWindow(m_hwnd, SW_SHOWDEFAULT);

    create_webview(m_hwnd);

    PostMessageW(parentHwnd, WM_SIZE, 0, 0);
}

WebView::~WebView() {}

__int64 __stdcall WebView::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    WebView* webview = InstanceFromWndProc<WebView, WebView, &WebView::m_hwnd>(hwnd, msg, lparam);

    if (webview)
    {
        switch (msg)
        {
        case WM_WINDOWPOSCHANGED:
        {
            RECT r;
            GetClientRect(hwnd, &r);

            if (webview->controller)
            {
                webview->controller->put_Bounds(r);
            }

            return 0;
        }
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

void WebView::create_webview(HWND childHwnd)
{
    // SetEnvironmentVariableW(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR", L"0");
    SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            L"--allow-file-access-from-files");

    winrt::check_hresult(CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [=, this](HRESULT, ICoreWebView2Environment* e) -> HRESULT
            {
                if (e)
                    winrt::check_hresult(e->CreateCoreWebView2Controller(
                        childHwnd,
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
                                    GetClientRect(childHwnd, &bounds);
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
                                }

                                return S_OK;
                            })
                            .Get()));

                return S_OK;
            })
            .Get()));
}
