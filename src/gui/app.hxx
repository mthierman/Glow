#pragma once

#include <Windows.h>
#include <string>

#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.viewmanagement.h>
#include <wrl.h>
#include <WebView2.h>

#include "../helpers/helpers.hxx"

namespace glow
{
class App
{
  public:
    App(std::string, bool, Bounds);
    ~App();

    HWND get_hwnd();
    void* get_hwnd_void();
    void make_child(std::string, Bounds);

  private:
    static __int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
    static int __stdcall EnumChildProc(HWND, LPARAM);
    static __int64 __stdcall WebViewWndProc(HWND, UINT, WPARAM, LPARAM);

    bool create_webview(HWND);
    bool create_controller(HWND, ICoreWebView2Environment*);
    bool get_core(ICoreWebView2Controller*);
    bool get_settings(ICoreWebView2*);

    winrt::com_ptr<ICoreWebView2Controller> controller;
    winrt::com_ptr<ICoreWebView2Controller4> controller4;
    winrt::com_ptr<ICoreWebView2> core;
    winrt::com_ptr<ICoreWebView2_19> core19;
    winrt::com_ptr<ICoreWebView2Settings> settings;

    std::string appName;
    std::string randomName;
    Bounds bounds;
    ATOM atom;
    HWND hwnd;
};

App::App(std::string n, bool plugin, Bounds b)
    : appName(n), randomName(glow::win32::randomize(n)), bounds(b)
{
    auto wideName{glow::win32::to_wstring(appName)};
    auto wideRandomName{glow::win32::to_wstring(randomName)};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = wideRandomName.c_str();
    wcex.lpszMenuName = wideRandomName.c_str();
    wcex.lpfnWndProc = App::WndProc;
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

    atom = RegisterClassExW(&wcex);

    if (atom == 0)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    hwnd = CreateWindowExW(0, wideRandomName.c_str(), wideName.c_str(),
                           plugin ? WS_POPUP | WS_CLIPCHILDREN | WS_CLIPCHILDREN
                                  : WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPCHILDREN,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                           nullptr, GetModuleHandleW(nullptr), this);

    if (!hwnd)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    if (!(bounds.width == 0) || !(bounds.height == 0))
        SetWindowPos(hwnd, nullptr, bounds.x, bounds.y, bounds.width, bounds.height, 0);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
}

App::~App() {}

__int64 __stdcall App::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    App* app = InstanceFromWndProc<App>(hwnd, msg, lparam);

    if (app)
    {
        switch (msg)
        {
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_SIZE:
        {
            RECT r;
            GetClientRect(hwnd, &r);
            EnumChildWindows(hwnd, EnumChildProc, (LPARAM)&r);

            return 0;
        }
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

int __stdcall App::EnumChildProc(HWND hwndChild, LPARAM lparam)
{
    auto child{GetWindowLongPtrW(hwndChild, GWL_ID)};
    auto p{(LPRECT)lparam};

    SetWindowPos(hwndChild, nullptr, 0, 0, p->right, p->bottom, SWP_NOZORDER);

    return TRUE;
}

__int64 __stdcall App::WebViewWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    App* app = InstanceFromWndProc<App>(hwnd, msg, lparam);

    if (app)
    {
        switch (msg)
        {
        case WM_WINDOWPOSCHANGED:
        {
            RECT r;
            GetClientRect(hwnd, &r);

            if (app->controller)
            {
                app->controller->put_Bounds(r);
            }

            return 0;
        }
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

HWND App::get_hwnd() { return hwnd; }

void* App::get_hwnd_void() { return (void*)hwnd; }

void App::make_child(std::string n, Bounds b)
{
    auto r{glow::win32::randomize(n)};

    auto wideName{glow::win32::to_wstring(n)};
    auto wideRandomName{glow::win32::to_wstring(r)};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = wideRandomName.c_str();
    wcex.lpszMenuName = wideRandomName.c_str();
    wcex.lpfnWndProc = App::WebViewWndProc;
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

    auto childAtom{RegisterClassExW(&wcex)};

    if (childAtom == 0)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    auto childHwnd{CreateWindowExW(WS_EX_TRANSPARENT, wideRandomName.c_str(), wideName.c_str(),
                                   WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                   CW_USEDEFAULT, hwnd, nullptr, GetModuleHandleW(nullptr), this)};

    if (!childHwnd)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    create_webview(childHwnd);

    ShowWindow(childHwnd, SW_SHOWDEFAULT);

    PostMessageW(hwnd, WM_SIZE, 0, 0);
}

bool App::create_webview(HWND childHwnd)
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

bool App::create_controller(HWND childHwnd, ICoreWebView2Environment* e)
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
                            core19->Navigate(L"https://www.google.com/");
                            get_settings(core19.get());
                        }
                    }

                    return S_OK;
                })
                .Get())))

        return true;

    else
        return false;
}

bool App::get_core(ICoreWebView2Controller* c)
{
    if (SUCCEEDED(c->get_CoreWebView2(core.put())))
        return true;

    else
        return false;
}

bool App::get_settings(ICoreWebView2* c)
{
    if (SUCCEEDED(c->get_Settings(settings.put())))
    {
        settings->put_AreDefaultContextMenusEnabled(true);
        settings->put_AreDefaultScriptDialogsEnabled(true);
        settings->put_AreHostObjectsAllowed(true);
        settings->put_IsBuiltInErrorPageEnabled(true);
        settings->put_IsScriptEnabled(true);
        settings->put_IsStatusBarEnabled(true);
        settings->put_IsWebMessageEnabled(true);
        settings->put_IsZoomControlEnabled(true);
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
