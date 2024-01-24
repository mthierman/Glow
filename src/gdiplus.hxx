// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <stdexcept>

namespace glow
{
namespace gui
{
struct GdiPlus
{
    GdiPlus();
    ~GdiPlus();

    uintptr_t token{};
    Gdiplus::GdiplusStartupInput input{};
    Gdiplus::Status status{Gdiplus::GdiplusStartup(&token, &input, nullptr)};
};
} // namespace gui
} // namespace glow
