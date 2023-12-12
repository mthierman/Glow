// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <DispatcherQueue.h>
#include <Unknwn.h>
#include <wrl.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Composition.Desktop.h>

#include <windows.ui.composition.interop.h>

#include <winrt/Microsoft.Web.WebView2.Core.h>

#include <string>

#include <gui/gui.hxx>
#include <text/text.hxx>

//==============================================================================
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

//==============================================================================
namespace glow::gui
{

//==============================================================================
class CompositionHost
{
  public:
    ~CompositionHost();
    static CompositionHost* GetInstance();

    //==============================================================================
    auto Initialize(HWND hwnd) -> void;
    // auto AddElement(float size, float x, float y) -> void;

    //==============================================================================
    winrt::Compositor m_compositor{nullptr};
    winrt::ContainerVisual m_container{nullptr};
    winrt::DesktopWindowTarget m_target{nullptr};
    winrt::DispatcherQueueController m_dispatcherQueueController{nullptr};

  private:
    CompositionHost();

    //==============================================================================
    auto EnsureDispatcherQueue() -> void;
    auto CreateDesktopWindowTarget(HWND window) -> void;
    auto CreateCompositionRoot() -> void;
};

//==============================================================================
struct WebViewComp
{
    WebViewComp(std::string name, HWND hwnd, int id);
    ~WebViewComp();

    //==============================================================================
    auto create_webview() -> winrt::IAsyncAction;

    //==============================================================================
    static auto CALLBACK wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto on_window_pos_changed() -> int;

    //==============================================================================
    wil::unique_hwnd m_hwndParent{nullptr};
    wil::unique_hwnd m_hwnd{nullptr};
    std::string m_class;
    UINT_PTR m_id{0};
    bool m_initialized{false};

    //==============================================================================
    winrt::CoreWebView2Environment environment{nullptr};
    winrt::CoreWebView2Profile profile{nullptr};
    winrt::CoreWebView2Controller controller{nullptr};
    winrt::CoreWebView2CompositionController comp{nullptr};
    winrt::CoreWebView2 core{nullptr};
};

//==============================================================================
} // namespace glow::gui
