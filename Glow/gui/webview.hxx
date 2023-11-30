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
#include <wrl.h>

#include <winrt/Windows.Foundation.h>

#include <WebView2.h>

#include <string>

#include <gui/gui.hxx>
#include <text/text.hxx>

namespace glow::gui
{
struct WebView
{
  public:
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
    winrt::com_ptr<ICoreWebView2Controller> controller{nullptr};
    winrt::com_ptr<ICoreWebView2Controller4> controller4{nullptr};
    winrt::com_ptr<ICoreWebView2> core{nullptr};
    winrt::com_ptr<ICoreWebView2_19> core19{nullptr};
    winrt::com_ptr<ICoreWebView2Settings> settings{nullptr};
};
} // namespace glow::gui
