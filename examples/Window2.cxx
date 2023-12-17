// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <iostream>

#include <gui/window.hxx>

auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    glow::gui::Window window;

    std::cout << sizeof(window) << std::endl;
    std::cout << sizeof(glow::gui::Window) << std::endl;

    std::cout << sizeof(nullptr) << std::endl;

    glow::gui::message_loop();

    return 0;
}
