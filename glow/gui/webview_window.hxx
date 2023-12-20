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

#include <text/text.hxx>
#include <gui/window.hxx>

namespace glow::gui
{

struct WebViewWindow final : public glow::gui::Window
{
    // using glow::gui::Window::Window;
    WebViewWindow(std::string name, HWND parentHwnd, int64_t id);

    auto register_class() -> ATOM;
    auto create_window() -> HWND override;
    auto create_environment() -> void;
    auto create_controller(ICoreWebView2Environment* environment) -> void;

    std::string m_title{"Test"};
    std::string m_class;
    int64_t m_id{};
    wil::unique_hwnd m_hwndParent;

    winrt::com_ptr<ICoreWebView2EnvironmentOptions6> m_evironmentOptions6{nullptr};
    winrt::com_ptr<ICoreWebView2Controller> m_controller{nullptr};
    winrt::com_ptr<ICoreWebView2Controller4> m_controller4{nullptr};
    winrt::com_ptr<ICoreWebView2> m_core{nullptr};
    winrt::com_ptr<ICoreWebView2_20> m_core20{nullptr};
    winrt::com_ptr<ICoreWebView2Settings> m_settings{nullptr};
    winrt::com_ptr<ICoreWebView2Settings8> m_settings8{nullptr};
    wil::unique_hbrush m_lightHbrBackground{static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH))};
};

} // namespace glow::gui
