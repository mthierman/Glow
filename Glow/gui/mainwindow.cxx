// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <gui/mainwindow.hxx>

namespace glow::gui
{

auto MainWindow::on_destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> int
{
    PostQuitMessage(0);

    return 0;
}

} // namespace glow::gui
