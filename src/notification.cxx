// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "notification.hxx"

namespace glow
{
auto Notification::reset() -> void
{
    nmhdr.hwndFrom = nullptr;
    nmhdr.idFrom = 0;
    nmhdr.code = 0;

    hwnd = nullptr;
    id = 0;
    code = static_cast<CODE>(0);
    message.clear();
}
} // namespace glow
