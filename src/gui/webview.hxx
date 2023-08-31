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
    bool create_webview(HWND);
    bool create_controller(HWND, ICoreWebView2Environment*);
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

bool WebView::create_webview(HWND childHwnd)
{
    // SetEnvironmentVariableW(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR", L"0");
    SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            L"--allow-file-access-from-files");

    if (SUCCEEDED(CreateCoreWebView2EnvironmentWithOptions(
            nullptr, nullptr, nullptr,
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [=, this](HRESULT, ICoreWebView2Environment* e) -> HRESULT
                {
                    if (e)
                    {
                        create_controller(childHwnd, e);
                    }

                    return S_OK;
                })
                .Get())))

        return true;

    else
        return false;
}

bool WebView::create_controller(HWND childHwnd, ICoreWebView2Environment* e)
{
    if (SUCCEEDED(e->CreateCoreWebView2Controller(
            childHwnd,
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
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

                        if (get_core(controller4.get()))
                        {
                            core19 = core.as<ICoreWebView2_19>();
                            get_settings(core19.get());

                            // auto widePath{L"file:///" + sitePath.wstring()};
                            // core19->Navigate(widePath.c_str());
                            core19->Navigate(L"about:blank");
                        }
                    }

                    return S_OK;
                })
                .Get())))

        return true;

    else
        return false;
}

bool WebView::get_core(ICoreWebView2Controller* c)
{
    if (SUCCEEDED(c->get_CoreWebView2(core.put())))
        return true;

    else
        return false;
}

bool WebView::get_settings(ICoreWebView2* c)
{
    if (SUCCEEDED(c->get_Settings(settings.put())))
    {
        settings->put_AreDefaultContextMenusEnabled(true);
        settings->put_AreDefaultScriptDialogsEnabled(true);
        settings->put_AreHostObjectsAllowed(true);
        settings->put_IsBuiltInErrorPageEnabled(true);
        settings->put_IsScriptEnabled(true);
        settings->put_IsStatusBarEnabled(false);
        settings->put_IsWebMessageEnabled(true);
        settings->put_IsZoomControlEnabled(false);
#ifdef _DEBUG
        settings->put_AreDevToolsEnabled(true);
#else
        settings->put_AreDevToolsEnabled(false);
#endif

        return true;
    }

    else
        return false;
}
} // namespace glow
