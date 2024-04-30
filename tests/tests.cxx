// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/config.hxx>
#include <glow/glow.hxx>

#include <print>

auto main(int argc, char* argv[]) -> int
{
    glow::Colors colors;

    std::println("{}", colors.string.accent);
    std::println("{}", colors.string.accentDark1);
    std::println("{}", colors.string.accentDark2);
    std::println("{}", colors.string.accentDark3);
    std::println("{}", colors.string.accentLight1);
    std::println("{}", colors.string.accentLight2);
    std::println("{}", colors.string.accentLight3);

    std::println("{}", colors.string.foreground);
    std::println("{}", colors.string.background);

    return 0;
}
