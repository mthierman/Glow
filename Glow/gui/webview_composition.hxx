// clang-format off
// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <DispatcherQueue.h>
#include <Unknwn.h>
#include <wrl.h>

#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Composition.Desktop.h>

#include <windows.ui.composition.interop.h>

#include <winrt/Microsoft.Web.WebView2.Core.h>

#include <string>

#include <gui/gui.hxx>
#include <text/text.hxx>

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

namespace glow::gui
{
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

struct WebViewComp
{
    WebViewComp(std::string, HWND, int);
    ~WebViewComp();

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
} // namespace glow::gui
