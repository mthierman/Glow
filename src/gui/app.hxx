#pragma once

#include <Windows.h>
#include <string>

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
    static __int64 __stdcall WebViewWndProc(HWND, UINT, WPARAM, LPARAM);
    static int __stdcall EnumChildProc(HWND, LPARAM);

    bool create_webview(HWND);
    bool create_controller(HWND, ICoreWebView2Environment*);
    bool get_core(ICoreWebView2Controller*);
    bool get_settings(ICoreWebView2*);
    void navigate();
    void put_bounds(RECT);

    Microsoft::WRL::ComPtr<ICoreWebView2Environment> webviewEnvironment;
    Microsoft::WRL::ComPtr<ICoreWebView2Controller> webviewController;
    Microsoft::WRL::ComPtr<ICoreWebView2> webviewCore;
    Microsoft::WRL::ComPtr<ICoreWebView2Settings> webviewSettings;

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

    hwnd =
        CreateWindowExW(0, wideRandomName.c_str(), wideName.c_str(),
                        plugin ? WS_POPUP : WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPCHILDREN,
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

__int64 __stdcall App::WebViewWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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

            if (app->webviewController)
            {
                app->webviewController->put_Bounds(r);
            }

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

    RECT test;
    GetClientRect(hwndChild, &test);

    // MoveWindow(hwndChild, 0, 0, p->right / 2, p->bottom, TRUE);

    SetWindowPos(hwndChild, nullptr, 0, 0, p->right / 2, p->bottom, SWP_NOACTIVATE);

    // MoveWindow(hwndChild, 0, 0, test.right / 2, test.bottom, TRUE);

    return TRUE;
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

    auto childHwnd{CreateWindowExW(0, wideRandomName.c_str(), wideName.c_str(), WS_CHILD,
                                   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd,
                                   nullptr, GetModuleHandleW(nullptr), this)};

    if (!childHwnd)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    // auto bgBrush{(HBRUSH)GetStockObject(WHITE_BRUSH)};
    // SetClassLongPtrW(childHwnd, GCLP_HBRBACKGROUND, (LONG_PTR)bgBrush);

    ShowWindow(childHwnd, SW_SHOWDEFAULT);

    create_webview(childHwnd);
}

bool App::create_webview(HWND childHwnd)
{
    SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            L"--allow-file-access-from-files");

    if (SUCCEEDED(CreateCoreWebView2EnvironmentWithOptions(
            nullptr, nullptr, nullptr,
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [=, this](HRESULT, ICoreWebView2Environment* e) -> HRESULT
                {
                    if (e)
                    {
                        webviewEnvironment = e;
                        create_controller(childHwnd, webviewEnvironment.Get());
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
                        webviewController = c;
                        RECT bounds{0, 0, 0, 0};
                        GetClientRect(childHwnd, &bounds);
                        webviewController->put_Bounds(bounds);

                        if (get_core(webviewController.Get()))
                        {
                            webviewCore.Get()->Navigate(L"https://google.com/");
                            get_settings(webviewCore.Get());
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
    if (SUCCEEDED(c->get_CoreWebView2(&webviewCore)))
        return true;

    else
        return false;
}

bool App::get_settings(ICoreWebView2* c)
{
    if (SUCCEEDED(c->get_Settings(&webviewSettings)))
    {
        webviewSettings->put_AreDefaultContextMenusEnabled(false);
        webviewSettings->put_AreDefaultScriptDialogsEnabled(true);
        webviewSettings->put_AreHostObjectsAllowed(true);
        webviewSettings->put_IsBuiltInErrorPageEnabled(true);
        webviewSettings->put_IsScriptEnabled(true);
        webviewSettings->put_IsStatusBarEnabled(false);
        webviewSettings->put_IsWebMessageEnabled(true);
        webviewSettings->put_IsZoomControlEnabled(false);
#ifdef _DEBUG
        webviewSettings->put_AreDevToolsEnabled(true);
#else
        webviewSettings->put_AreDevToolsEnabled(false);
#endif

        return true;
    }

    else
        return false;
}

void App::navigate() { webviewCore.Get()->Navigate(L"http://localhost:8000/"); }

void App::put_bounds(RECT r)
{
    if (webviewController)
    {
        webviewController->put_Bounds(r);
    }
}
} // namespace glow
