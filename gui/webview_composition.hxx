#pragma once

#include <Windows.h>
#include <wrl.h>
#include <Unknwn.h>
#include <winrt/base.h>
#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.composition.desktop.h>
#include <windows.ui.composition.interop.h>
#include <DispatcherQueue.h>

#include "winrt/Microsoft.Web.WebView2.Core.h"

#include <string>

namespace winrt
{
using namespace winrt::Microsoft::Web::WebView2::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::System;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Composition;
using namespace winrt::Windows::UI::Composition::Desktop;
}; // namespace winrt

class CompositionHost
{
  public:
    ~CompositionHost();
    static CompositionHost* GetInstance();

    void Initialize(HWND hwnd);
    void AddElement(float size, float x, float y);

    winrt::Compositor m_compositor{nullptr};
    winrt::ContainerVisual m_container{nullptr};
    winrt::DesktopWindowTarget m_target{nullptr};
    winrt::DispatcherQueueController m_dispatcherQueueController{nullptr};

  private:
    CompositionHost();

    void CreateDesktopWindowTarget(HWND window);
    void EnsureDispatcherQueue();
    void CreateCompositionRoot();
};

CompositionHost::CompositionHost() {}

CompositionHost* CompositionHost::GetInstance()
{
    static CompositionHost instance;
    return &instance;
}

CompositionHost::~CompositionHost() {}

void CompositionHost::Initialize(HWND hwnd)
{
    EnsureDispatcherQueue();
    if (m_dispatcherQueueController) m_compositor = winrt::Compositor();

    if (m_compositor)
    {
        CreateDesktopWindowTarget(hwnd);
        CreateCompositionRoot();
        m_container = m_compositor.CreateContainerVisual();
    }
}

void CompositionHost::EnsureDispatcherQueue()
{
    namespace abi = ABI::Windows::System;

    if (m_dispatcherQueueController == nullptr)
    {
        DispatcherQueueOptions options{
            sizeof(DispatcherQueueOptions), /* dwSize */
            DQTYPE_THREAD_CURRENT,          /* threadType */
            DQTAT_COM_ASTA                  /* apartmentType */
        };

        winrt::DispatcherQueueController controller{nullptr};
        winrt::check_hresult(CreateDispatcherQueueController(
            options,
            reinterpret_cast<abi::IDispatcherQueueController**>(winrt::put_abi(controller))));
        m_dispatcherQueueController = controller;
    }
}

void CompositionHost::CreateDesktopWindowTarget(HWND window)
{
    namespace abi = ABI::Windows::UI::Composition::Desktop;

    auto interop = m_compositor.as<abi::ICompositorDesktopInterop>();
    winrt::DesktopWindowTarget target{nullptr};
    winrt::check_hresult(interop->CreateDesktopWindowTarget(
        window, false, reinterpret_cast<abi::IDesktopWindowTarget**>(winrt::put_abi(target))));
    m_target = target;
}

void CompositionHost::CreateCompositionRoot()
{
    auto root = m_compositor.CreateContainerVisual();
    root.RelativeSizeAdjustment({1.0f, 1.0f});
    root.Offset({0, 0, 0});

    m_target.Root(root);
}

namespace glow::gui
{
struct WebView
{
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

    winrt::IAsyncAction create_webview(HWND);

    winrt::CoreWebView2Environment environment{nullptr};
    winrt::CoreWebView2Profile profile{nullptr};
    winrt::CoreWebView2Controller controller{nullptr};
    winrt::CoreWebView2CompositionController comp{nullptr};
    winrt::CoreWebView2 core{nullptr};
};

winrt::IAsyncAction WebView::create_webview(HWND h)
{
    CompositionHost* compHost = CompositionHost::GetInstance();
    compHost->Initialize(webviewHwnd);

    auto windowRef{winrt::CoreWebView2ControllerWindowReference::CreateFromWindowHandle(
        reinterpret_cast<uint64_t>(h))};
    auto environmentOptions{winrt::CoreWebView2EnvironmentOptions()};
    environmentOptions.AdditionalBrowserArguments(L"--allow-file-access-from-files");
    environment = co_await winrt::CoreWebView2Environment::CreateWithOptionsAsync(
        L"", L"", environmentOptions);
    comp = co_await environment.CreateCoreWebView2CompositionControllerAsync(windowRef);

    comp.RootVisualTarget(compHost->m_target.Root());

    comp.as(controller);
    core = controller.CoreWebView2();
    auto webviewSettings{core.Settings()};
    webviewSettings.AreBrowserAcceleratorKeysEnabled(true);
    webviewSettings.AreDefaultContextMenusEnabled(true);
    webviewSettings.AreDefaultScriptDialogsEnabled(true);
    webviewSettings.AreDevToolsEnabled(true);
    webviewSettings.AreHostObjectsAllowed(true);
    webviewSettings.IsBuiltInErrorPageEnabled(true);
    webviewSettings.IsGeneralAutofillEnabled(true);
    webviewSettings.IsPasswordAutosaveEnabled(true);
    webviewSettings.IsPinchZoomEnabled(true);
    webviewSettings.IsReputationCheckingRequired(true);
    webviewSettings.IsScriptEnabled(true);
    webviewSettings.IsStatusBarEnabled(true);
    webviewSettings.IsSwipeNavigationEnabled(true);
    webviewSettings.IsWebMessageEnabled(true);
    webviewSettings.IsZoomControlEnabled(true);
    core.Navigate(winrt::to_hstring(std::string("https://www.google.com/")));

    RECT bounds{0, 0, 0, 0};
    GetClientRect(h, &bounds);
    winrt::Windows::Foundation::Rect displayWebView{
        static_cast<float>(bounds.left), static_cast<float>(bounds.top),
        static_cast<float>(bounds.right), static_cast<float>(bounds.bottom)};
    controller.Bounds(displayWebView);
}

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
    if (controller)
    {
        RECT bounds{0, 0, 0, 0};
        GetClientRect(h, &bounds);
        winrt::Windows::Foundation::Rect displayWebView{
            static_cast<float>(bounds.left), static_cast<float>(bounds.top),
            static_cast<float>(bounds.right), static_cast<float>(bounds.bottom)};
        controller.Bounds(displayWebView);
    }

    return 0;
}
} // namespace glow::gui
