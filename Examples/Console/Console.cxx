// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <config/glow.hxx>
#include <console/console.hxx>
#include <filesystem/filesystem.hxx>
#include <gui/gui.hxx>
#include <gui/mainwindow.hxx>
#include <gui/webview.hxx>
#include <gui/window.hxx>
#include <log/log.hxx>
#include <text/text.hxx>

auto main() -> int
{
    glow::log::debug("test");
    glow::log::print("test");
    // glow::log::box("test");
    // glow::log::shell("test");
    // glow::log::stock("test");

    return 0;
}
