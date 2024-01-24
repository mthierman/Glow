// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/config.hxx>
#include <glow/glow.hxx>

auto main(int argc, char* argv[]) -> int
{
    glow::console::source_print("🌸🌹🍒🍉🥝🍎🖍️🧶");

    auto cppwinrt{glow::config::cppwinrt};

    glow::console::source_print(cppwinrt);

    return 0;
}
