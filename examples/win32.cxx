// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/config.hxx>
#include <glow/glow.hxx>

// #include <iostream>

auto main() -> int
{
    glow::console::Console console;

    std::cout << "Test" << std::endl;

    return glow::gui::message_loop();
}
