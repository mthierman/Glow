#include <Windows.h>
#include <wrl.h>
#include <Unknwn.h>
#include <winrt/base.h>
#include <winrt/windows.ui.composition.desktop.h>
#include <windows.ui.composition.interop.h>
#include <DispatcherQueue.h>

#include "WebView2.h"

#include "../helpers/Helpers.hxx"
#include "../gui/App.hxx"

using namespace winrt;
using namespace winrt::Windows::System;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Composition;
using namespace winrt::Windows::UI::Composition::Desktop;
using namespace winrt::Windows::Foundation::Numerics;

winrt::com_ptr<ICoreWebView2Environment> env{nullptr};
winrt::com_ptr<ICoreWebView2Environment12> env12{nullptr};
winrt::com_ptr<ICoreWebView2CompositionController> compController{nullptr};
winrt::com_ptr<ICoreWebView2CompositionController3> compController3{nullptr};
winrt::com_ptr<ICoreWebView2Controller> controller{nullptr};
winrt::com_ptr<ICoreWebView2Controller4> controller4{nullptr};
winrt::com_ptr<ICoreWebView2> core{nullptr};
winrt::com_ptr<ICoreWebView2_19> core19{nullptr};
winrt::com_ptr<ICoreWebView2Settings> settings{nullptr};

class CompositionHost
{
  public:
    ~CompositionHost();
    static CompositionHost* GetInstance();

    void Initialize(HWND hwnd);
    void AddElement(float size, float x, float y);

  private:
    CompositionHost();

    void CreateDesktopWindowTarget(HWND window);
    void EnsureDispatcherQueue();
    void CreateCompositionRoot();

    winrt::Windows::UI::Composition::Compositor m_compositor{nullptr};
    winrt::Windows::UI::Composition::Desktop::DesktopWindowTarget m_target{nullptr};
    winrt::Windows::System::DispatcherQueueController m_dispatcherQueueController{nullptr};
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
    if (m_dispatcherQueueController) m_compositor = Compositor();

    if (m_compositor)
    {
        CreateDesktopWindowTarget(hwnd);
        CreateCompositionRoot();
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

        DispatcherQueueController controller{nullptr};
        check_hresult(CreateDispatcherQueueController(
            options, reinterpret_cast<abi::IDispatcherQueueController**>(put_abi(controller))));
        m_dispatcherQueueController = controller;
    }
}

void CompositionHost::CreateDesktopWindowTarget(HWND window)
{
    namespace abi = ABI::Windows::UI::Composition::Desktop;

    auto interop = m_compositor.as<abi::ICompositorDesktopInterop>();
    DesktopWindowTarget target{nullptr};
    check_hresult(interop->CreateDesktopWindowTarget(
        window, false, reinterpret_cast<abi::IDesktopWindowTarget**>(put_abi(target))));
    m_target = target;
}

void CompositionHost::CreateCompositionRoot()
{
    auto root = m_compositor.CreateContainerVisual();
    root.RelativeSizeAdjustment({1.0f, 1.0f});
    root.Offset({124, 12, 0});
    m_target.Root(root);
}

void CompositionHost::AddElement(float size, float x, float y)
{
    if (m_target.Root())
    {
        auto visuals = m_target.Root().as<ContainerVisual>().Children();
        auto visual = m_compositor.CreateSpriteVisual();

        auto element = m_compositor.CreateSpriteVisual();
        uint8_t r = (double)(double)(rand() % 255);
        ;
        uint8_t g = (double)(double)(rand() % 255);
        ;
        uint8_t b = (double)(double)(rand() % 255);
        ;

        element.Brush(m_compositor.CreateColorBrush({255, r, g, b}));
        element.Size({size, size});
        element.Offset({
            x,
            y,
            0.0f,
        });

        auto animation = m_compositor.CreateVector3KeyFrameAnimation();
        auto bottom = (float)600 - element.Size().y;
        animation.InsertKeyFrame(1, {element.Offset().x, bottom, 0});

        using timeSpan = std::chrono::duration<int, std::ratio<1, 1>>;

        std::chrono::seconds duration(2);
        std::chrono::seconds delay(3);

        animation.Duration(timeSpan(duration));
        animation.DelayTime(timeSpan(delay));
        element.StartAnimation(L"Offset", animation);
        visuals.InsertAtTop(element);

        visuals.InsertAtTop(visual);
    }
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/,
                    int /*nCmdShow*/)
{
    glow::gui::App app("Glow");
    SetWindowPos(app.appHwnd, 0, 100, 100, 900, 700, 0);

    CompositionHost* compHost = CompositionHost::GetInstance();

    compHost->Initialize(app.appHwnd);

    double size = (double)(rand() % 150 + 50);
    double x = (double)(rand() % 600);
    double y = (double)(rand() % 200);
    compHost->AddElement(size, x, y);

    winrt::check_hresult(CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [=](HRESULT, ICoreWebView2Environment* e) -> HRESULT
            {
                if (e)
                {
                    env.attach(e);
                    env12 = env.as<ICoreWebView2Environment12>();
                    winrt::check_hresult(env12->CreateCoreWebView2CompositionController(
                        app.appHwnd,
                        Microsoft::WRL::Callback<
                            ICoreWebView2CreateCoreWebView2CompositionControllerCompletedHandler>(
                            [=](HRESULT, ICoreWebView2CompositionController* c) -> HRESULT
                            {
                                if (c)
                                {
                                    compController.attach(c);
                                    compController3 =
                                        controller.as<ICoreWebView2CompositionController3>();

                                    controller = compController3.as<ICoreWebView2Controller>();
                                    controller4 = controller.as<ICoreWebView2Controller4>();
                                    COREWEBVIEW2_COLOR bgColor{0, 0, 0, 0};

                                    controller4->put_DefaultBackgroundColor(bgColor);

                                    RECT bounds{0, 0, 0, 0};
                                    GetClientRect(app.appHwnd, &bounds);
                                    controller4->put_Bounds(bounds);

                                    winrt::check_hresult(controller4->get_CoreWebView2(core.put()));

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
                }
            })
            .Get()));

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
