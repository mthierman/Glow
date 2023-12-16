// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <Windows.h>

#include <memory>
#include <print>

#include <console/console.hxx>
#include <gui/app.hxx>
#include <gui/gui.hxx>
#include <gui/webview.hxx>

#include "WebView.hxx"

auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
#ifdef _DEBUG
    glow::console::Console console;
#endif

    auto app{std::make_unique<glow::App>("WebView")};

    auto wv{std::make_unique<glow::gui::WebView>("WebView", app->m_hwnd.get(), 1)};

    std::println("COUT");
    std::println(stderr, "CERR");

    glow::gui::message_loop();

    return 0;
}
