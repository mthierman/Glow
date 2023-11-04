#include <Windows.h>
#include <wrl.h>
#include <Unknwn.h>
#include <winrt/base.h>
#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.composition.desktop.h>
#include <windows.ui.composition.interop.h>
#include <DispatcherQueue.h>

#include "winrt/Microsoft.Web.WebView2.Core.h"

#include "../helpers/Helpers.hxx"
#include "../gui/App.hxx"

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

winrt::CoreWebView2Environment environment{nullptr};
winrt::CoreWebView2Profile profile{nullptr};
winrt::CoreWebView2Controller controller{nullptr};
winrt::CoreWebView2CompositionController comp{nullptr};
winrt::CoreWebView2 core{nullptr};

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
    // root.RotationAngle(70);
    root.BorderMode(winrt::CompositionBorderMode::Soft);

    m_target.Root(root);
}

winrt::IAsyncAction create_webview(HWND h, CompositionHost* c)
{
    auto windowRef{winrt::CoreWebView2ControllerWindowReference::CreateFromWindowHandle(
        reinterpret_cast<uint64_t>(h))};
    auto environmentOptions{winrt::CoreWebView2EnvironmentOptions()};
    environmentOptions.AdditionalBrowserArguments(L"--allow-file-access-from-files");
    environment = co_await winrt::CoreWebView2Environment::CreateWithOptionsAsync(
        L"", L"", environmentOptions);
    comp = co_await environment.CreateCoreWebView2CompositionControllerAsync(windowRef);
    // comp.

    comp.RootVisualTarget(c->m_target.Root());

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

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/,
                    int /*nCmdShow*/)
{
    glow::gui::App app("Glow");

    CompositionHost* compHost = CompositionHost::GetInstance();

    compHost->Initialize(app.appHwnd);

    create_webview(app.appHwnd, compHost);

    // m_compositor
    // m_container
    // m_target
    // m_dispatcherQueueController

    // if (comp) OutputDebugStringW(L"TEST");
    // comp.RootVisualTarget(nullptr);

    MSG msg;
    int r;

    while ((r = GetMessageW(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) return 0;

        else
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return 0;
}
