// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <Windows.h>

#include <print>

#include <console/console.hxx>
#include <gui/app.hxx>
#include <gui/gui.hxx>

auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    // #ifdef _DEBUG
    //     glow::console::Console console;
    // #endif

    // auto mainWindow{std::make_unique<glow::gui::App>("MainWindow")};
    // std::println("cout");
    // std::println(stderr, "cerr");

    glow::gui::App app{"App"};

    glow::gui::message_loop();

    return 0;
}
