// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "gdiplus.hxx"

namespace glow
{
namespace gui
{
GdiPlus::GdiPlus()
{
    if (status != Gdiplus::Status::Ok) { throw std::runtime_error("GDI+ startup failure"); }
}

GdiPlus::~GdiPlus()
{
    if (status == Gdiplus::Status::Ok) Gdiplus::GdiplusShutdown(token);
}
} // namespace gui
} // namespace glow
