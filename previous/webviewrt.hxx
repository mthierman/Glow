#pragma once

#include <Windows.h>
#include <string>
#include <filesystem>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.Web.WebView2.Core.h>
#include "../helpers/helpers.hxx"

namespace winrt
{
using namespace winrt::Microsoft::Web::WebView2::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI;
}; // namespace winrt

namespace glow
{
class WebView
{
  public:
    WebView(std::string, HWND);
    ~WebView();

    winrt::IAsyncAction create_webview();

    winrt::CoreWebView2Environment environment{nullptr};
    winrt::CoreWebView2Profile profile{nullptr};
    winrt::CoreWebView2Controller controller{nullptr};
    winrt::CoreWebView2 core{nullptr};

  protected:
    static __int64 __stdcall WndProcCallback(HWND, UINT, WPARAM, LPARAM);
    virtual __int64 WndProc(HWND, UINT, WPARAM, LPARAM);

    winrt::Rect panel_full(RECT);

    HWND m_hwnd;
    HWND parentHwnd;
};

WebView::WebView(std::string name, HWND h) : parentHwnd(h)
{
    auto r{glow::win32::randomize(name)};
    auto wideRandomName{glow::win32::to_wstring(r)};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = wideRandomName.c_str();
    wcex.lpszMenuName = wideRandomName.c_str();
    wcex.lpfnWndProc = WndProcCallback;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = ::GetModuleHandleW(nullptr);
    // wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    wcex.hCursor =
        (HCURSOR)::LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wcex.hIcon = (HICON)::LoadImageW(nullptr, (LPCWSTR)IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                     LR_SHARED | LR_DEFAULTSIZE);
    wcex.hIconSm = (HICON)::LoadImageW(nullptr, (LPCWSTR)IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                       LR_SHARED | LR_DEFAULTSIZE);

    auto atom{::RegisterClassExW(&wcex)};

    if (atom == 0)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    ::CreateWindowExW(WS_EX_TRANSPARENT, wideRandomName.c_str(), wideRandomName.c_str(), WS_CHILD,
                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parentHwnd,
                      nullptr, ::GetModuleHandleW(nullptr), this);

    if (!m_hwnd)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);

    // create_webview();
}

WebView::~WebView() {}

__int64 __stdcall WebView::WndProcCallback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    WebView* webview = InstanceFromWndProc<WebView, WebView, &WebView::m_hwnd>(hwnd, msg, lparam);

    if (webview)
    {
        return webview->WndProc(hwnd, msg, wparam, lparam);
    }

    return ::DefWindowProcW(hwnd, msg, wparam, lparam);
}

__int64 WebView::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_WINDOWPOSCHANGED:
        ::OutputDebugStringW(L"OVERRIDE");
        return 0;
    }

    return ::DefWindowProcW(hwnd, msg, wparam, lparam);
}

winrt::IAsyncAction WebView::create_webview()
{
    if (m_hwnd)
        auto windowRef{winrt::CoreWebView2ControllerWindowReference::CreateFromWindowHandle(
            reinterpret_cast<uint64_t>(m_hwnd))};

    else
        co_return;

    // auto environmentOptions{winrt::CoreWebView2EnvironmentOptions()};

    // environmentOptions.AdditionalBrowserArguments(L"--allow-file-access-from-files");

    // environment = co_await winrt::CoreWebView2Environment::CreateWithOptionsAsync(
    //     L"", L"", environmentOptions);

    // controller = co_await environment.CreateCoreWebView2ControllerAsync(windowRef);

    // auto bg{winrt::Color(0, 255, 255, 255)};
    // controller.DefaultBackgroundColor(bg);

    // core = controller.CoreWebView2();

    // auto webviewSettings{core.Settings()};

    // webviewSettings.AreBrowserAcceleratorKeysEnabled(true);
    // webviewSettings.AreDefaultContextMenusEnabled(true);
    // webviewSettings.AreDefaultScriptDialogsEnabled(true);
    // webviewSettings.AreDevToolsEnabled(true);
    // webviewSettings.AreHostObjectsAllowed(true);
    // webviewSettings.IsBuiltInErrorPageEnabled(true);
    // webviewSettings.IsGeneralAutofillEnabled(true);
    // webviewSettings.IsPasswordAutosaveEnabled(true);
    // webviewSettings.IsPinchZoomEnabled(true);
    // webviewSettings.IsReputationCheckingRequired(true);
    // webviewSettings.IsScriptEnabled(true);
    // webviewSettings.IsStatusBarEnabled(true);
    // webviewSettings.IsSwipeNavigationEnabled(true);
    // webviewSettings.IsWebMessageEnabled(true);
    // webviewSettings.IsZoomControlEnabled(true);

    // core.Navigate(winrt::hstring(L"https://www.google.com/").c_str());

    // PostMessageW(parentHwnd, WM_SIZE, 0, 0);
}

winrt::Rect WebView::panel_full(RECT bounds)
{
    return winrt::Rect{static_cast<float>(bounds.left), static_cast<float>(bounds.top),
                       static_cast<float>(bounds.right), static_cast<float>(bounds.bottom)};
}

// class WebView2 : public WebView
// {
//   public:
//     WebView2(std::string, HWND);
//     ~WebView2();
//     // winrt::CoreWebView2Environment environment{nullptr};
//     // winrt::CoreWebView2Profile profile{nullptr};
//     // winrt::CoreWebView2Controller controller{nullptr};
//     // winrt::CoreWebView2 core{nullptr};

//   protected:
//     __int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM) override;

//     // winrt::Rect panel_full(RECT);

//     HWND m_hwnd;
// };

// __int64 __stdcall WebView2::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
// {
//     WebView2* webview2 =
//         InstanceFromWndProc<WebView2, WebView2, &WebView2::m_hwnd>(hwnd, msg, lparam);

//     if (webview2)
//     {
//         switch (msg)
//         {
//         case WM_WINDOWPOSCHANGED:
//         {
//             OutputDebugStringW(L"OVERRIDE");
//             RECT r;
//             GetClientRect(hwnd, &r);

//             if (webview2->controller)
//             {
//                 webview2->controller.Bounds(webview2->panel_full(r));
//             }

//             return 0;
//         }
//         }
//     }

//     return DefWindowProcW(hwnd, msg, wparam, lparam);
// }
} // namespace glow
