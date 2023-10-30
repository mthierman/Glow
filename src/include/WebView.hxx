#include <Windows.h>
#include <wrl.h>
#include <windows.foundation.h>
#include "WebView2.h"
#include "Window.hxx"

class WebView : public glow::Window
{
  public:
    WebView(glow::Style, std::optional<HWND>, std::optional<int>);

    void create_webview();

    winrt::com_ptr<ICoreWebView2Controller> controller;
    winrt::com_ptr<ICoreWebView2Controller4> controller4;
    winrt::com_ptr<ICoreWebView2> core;
    winrt::com_ptr<ICoreWebView2_19> core19;
    winrt::com_ptr<ICoreWebView2Settings> settings;

  private:
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    int _OnWindowPosChanged(HWND, UINT, WPARAM, LPARAM);
};

WebView::WebView(glow::Style s, std::optional<HWND> h, std::optional<int> i) : glow::Window(s, h, i)
{
}

void WebView::create_webview()
{
    winrt::check_hresult(CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [=, this](HRESULT, ICoreWebView2Environment* e) -> HRESULT
            {
                if (e)
                    winrt::check_hresult(e->CreateCoreWebView2Controller(
                        m_hWnd, Microsoft::WRL::Callback<
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
                                            GetClientRect(m_hWnd, &bounds);
                                            controller4->put_Bounds(bounds);

                                            winrt::check_hresult(c->get_CoreWebView2(core.put()));

                                            core19 = core.as<ICoreWebView2_19>();

                                            winrt::check_hresult(
                                                core19->get_Settings(settings.put()));

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

LRESULT WebView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_WINDOWPOSCHANGED:
        return _OnWindowPosChanged(hWnd, uMsg, wParam, lParam);
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int WebView::_OnWindowPosChanged(HWND hWnd, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    RECT r;
    ::GetClientRect(hWnd, &r);

    if (controller4)
    {
        controller4->put_Bounds(r);
    }

    return 0;
}
