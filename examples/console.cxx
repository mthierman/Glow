// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/config.hxx>
#include <glow/glow.hxx>

auto main() -> int
{
    glow::console::source_print("🌸🌹🍒🍉🥝🍎🖍️🧶");
    glow::gui::SystemColors colors;

    std::println("{}", colors.string.accent);
    auto json{nlohmann::json{colors.map}};

    std::println("{}", json.dump());

    return 0;
}
