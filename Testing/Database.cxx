// clang-format off
// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝
// clang-format on

#include <print>

#include <console/console.hxx>
#include <filesystem/database.hxx>
#include <filesystem/filesystem.hxx>
#include <gui/app.hxx>
#include <gui/gui.hxx>
#include <gui/webview_composition.hxx>
#include <gui/webview.hxx>
#include <logging/logging.hxx>
#include <text/text.hxx>

auto main() -> int
{
    std::println("Test");

    glow::filesystem::Database db;

    db.write();

    return 0;
}