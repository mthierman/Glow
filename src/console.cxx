// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "console.hxx"

namespace glow
{
namespace console
{
Console::Console()
{
    AllocConsole();

    freopen_s(m_file.addressof(), "CONIN$", "r", stdin);
    freopen_s(m_file.addressof(), "CONOUT$", "w", stdout);
    freopen_s(m_file.addressof(), "CONOUT$", "w", stderr);

    std::cin.clear();
    std::cout.clear();
    std::cerr.clear();
    std::clog.clear();
}

Console::~Console() { FreeConsole(); }

auto Console::disable_close() -> void
{
    EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE,
                   MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
}
} // namespace console
} // namespace glow
