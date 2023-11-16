// clang-format off
// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝
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

#include <console/console.hxx>
#include <gui/app.hxx>
#include <gui/webview_composition.hxx>

#include "include/WebView.hxx"

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/,
                    int /*nCmdShow*/)
{
    // #ifdef _DEBUG
    //     glow::console::Console console;
    // #endif

    auto app{std::make_unique<glow::App>("WebViewComposition")};

    auto wv{std::make_unique<glow::gui::WebViewComp>("WebView", app->appHwnd, 1)};
    wv->create_webview(app->appHwnd);

    MSG msg;
    int r;

    while ((r = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) return 0;

        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}
