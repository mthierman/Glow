// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <nlohmann/json.hpp>

namespace glow
{
namespace gui
{
struct Position
{
    int x{};
    int y{};
    int width{};
    int height{};
    bool maximize{};
    bool fullscreen{};
    bool topmost{};
    int dpi{};
    float scale{};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y, width, height, maximize, fullscreen, topmost,
                                   dpi, scale)
};
} // namespace gui
} // namespace glow
