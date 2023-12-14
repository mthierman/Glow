// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <Windows.h>
#include <DispatcherQueue.h>
#include <Unknwn.h>
#include <wrl.h>

#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Composition.Desktop.h>

#include <windows.ui.composition.interop.h>

#include <winrt/Microsoft.Web.WebView2.Core.h>

#include <memory>

#include <console/console.hxx>
#include <gui/app.hxx>
#include <gui/gui.hxx>
#include <gui/webview_composition.hxx>

#include "WebView.hxx"

auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    // #ifdef _DEBUG
    //     glow::console::Console console;
    // #endif

    auto app{std::make_unique<glow::App>("WebViewComposition")};

    auto wv{std::make_unique<glow::gui::WebViewComp>("WebView", app->m_hwnd.get(), 1)};
    wv->create_webview();

    glow::gui::message_loop();

    return 0;
}
