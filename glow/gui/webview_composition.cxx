// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <gui/webview_composition.hxx>

namespace glow::gui
{

CompositionHost::CompositionHost() {}

CompositionHost* CompositionHost::GetInstance()
{
    static CompositionHost instance;
    return &instance;
}

CompositionHost::~CompositionHost() {}

auto CompositionHost::Initialize(HWND hwnd) -> void
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

auto CompositionHost::EnsureDispatcherQueue() -> void
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

auto CompositionHost::CreateDesktopWindowTarget(HWND window) -> void
{
    namespace abi = ABI::Windows::UI::Composition::Desktop;

    auto interop{m_compositor.as<abi::ICompositorDesktopInterop>()};
    winrt::DesktopWindowTarget target{nullptr};
    winrt::check_hresult(interop->CreateDesktopWindowTarget(
        window, false, reinterpret_cast<abi::IDesktopWindowTarget**>(winrt::put_abi(target))));
    m_target = target;
}

auto CompositionHost::CreateCompositionRoot() -> void
{
    auto root{m_compositor.CreateContainerVisual()};
    root.RelativeSizeAdjustment({1.0f, 1.0f});
    root.Offset({0, 0, 0});

    m_target.Root(root);
}

WebViewComp::WebViewComp(std::string name, HWND hwnd, int id) : m_hwndParent(hwnd), m_id(id)
{
    auto brush{reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH))};

    auto cursor{
        reinterpret_cast<HCURSOR>(::LoadImage(nullptr, reinterpret_cast<LPCSTR>(IDC_ARROW),
                                              IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    auto icon{
        reinterpret_cast<HICON>(::LoadImage(nullptr, reinterpret_cast<LPCSTR>(IDI_APPLICATION),
                                            IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

    m_class = glow::text::randomize(name);

    WNDCLASSEX wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = m_class.c_str();
    wcex.lpszMenuName = m_class.c_str();
    wcex.lpfnWndProc = WebViewComp::wnd_proc;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = ::GetModuleHandle(nullptr);
    wcex.hbrBackground = brush;
    wcex.hCursor = cursor;
    wcex.hIcon = icon;
    wcex.hIconSm = icon;

    ::RegisterClassEx(&wcex);

    ::CreateWindowEx(0, m_class.c_str(), name.c_str(), WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT,
                     CW_USEDEFAULT, CW_USEDEFAULT, m_hwndParent.get(),
                     reinterpret_cast<HMENU>(m_id), ::GetModuleHandle(nullptr), this);

    ::ShowWindow(m_hwnd.get(), SW_SHOW);
}

WebViewComp::~WebViewComp() {}

auto WebViewComp::create_webview() -> winrt::IAsyncAction
{
    CompositionHost* compHost{CompositionHost::GetInstance()};
    compHost->Initialize(m_hwnd.get());

    auto windowRef{winrt::CoreWebView2ControllerWindowReference::CreateFromWindowHandle(
        reinterpret_cast<uint64_t>(m_hwndParent.get()))};

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
    ::GetClientRect(m_hwndParent.get(), &bounds);
    winrt::Windows::Foundation::Rect displayWebView{
        static_cast<float>(bounds.left), static_cast<float>(bounds.top),
        static_cast<float>(bounds.right), static_cast<float>(bounds.bottom)};
    controller.Bounds(displayWebView);

    ::SendMessage(m_hwndParent.get(), WM_NOTIFY, 0, 0);
}

auto CALLBACK WebViewComp::wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto self{InstanceFromWndProc<WebViewComp>(hwnd, uMsg, lParam)};

    if (self)
    {
        switch (uMsg)
        {
        case WM_WINDOWPOSCHANGED: return self->on_window_pos_changed();
        }
    }

    return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

auto WebViewComp::on_window_pos_changed() -> int
{
    if (controller)
    {
        RECT rect{0, 0, 0, 0};
        ::GetClientRect(m_hwnd.get(), &rect);

        winrt::Windows::Foundation::Rect rectWv{
            static_cast<float>(rect.left), static_cast<float>(rect.top),
            static_cast<float>(rect.right), static_cast<float>(rect.bottom)};

        controller.Bounds(rectWv);
    }

    return 0;
}

} // namespace glow::gui
