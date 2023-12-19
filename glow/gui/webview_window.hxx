// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <wrl.h>
#include <winrt/Windows.Foundation.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <gui/window.hxx>

namespace glow::gui
{

struct WebViewWindow : glow::gui::Window
{
    using glow::gui::Window::Window;

    auto create_environment() -> void;
    auto create_controller(ICoreWebView2Environment* environment) -> void;

    winrt::com_ptr<ICoreWebView2EnvironmentOptions6> m_evironmentOptions6{nullptr};
    winrt::com_ptr<ICoreWebView2Controller> m_controller{nullptr};
    winrt::com_ptr<ICoreWebView2Controller4> m_controller4{nullptr};
    winrt::com_ptr<ICoreWebView2> m_core{nullptr};
    winrt::com_ptr<ICoreWebView2_20> m_core20{nullptr};
    winrt::com_ptr<ICoreWebView2Settings> m_settings{nullptr};
    winrt::com_ptr<ICoreWebView2Settings8> m_settings8{nullptr};
};

} // namespace glow::gui
