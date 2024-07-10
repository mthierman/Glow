// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/input.hxx>

namespace glow::input {
auto was_key_down(int key) -> bool { return (::GetKeyState(key) & 0x8000); }
}; // namespace glow::input
