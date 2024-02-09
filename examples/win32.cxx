// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/config.hxx>
#include <glow/glow.hxx>

struct App : public glow::App<App>
{
    using glow::App<App>::App;
};

struct Window : public glow::Window<Window>
{
    using glow::Window<Window>::Window;
};

auto main(int argc, char* argv[]) -> int
{
    App app;

    Window win;
    win.reveal();

    return app();
}
