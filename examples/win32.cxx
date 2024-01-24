// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/config.hxx>
#include <glow/glow.hxx>

struct Window : public glow::window::BaseWindow<Window>
{
    using glow::window::BaseWindow<Window>::BaseWindow;
};

auto main(int argc, char* argv[]) -> int
{
    Window win;
    win.reveal();

    glow::gui::GdiPlus gdiplus;
    glow::gui::CoInitialize conit;
    glow::gui::SystemColors systemColors;

    OutputDebugStringA(systemColors.string.accent.c_str());

    return glow::gui::message_loop();
}
