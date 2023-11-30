// clang-format off
// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <print>
#include <string>

#include <gui/gui.hxx>
#include <text/text.hxx>

namespace glow::gui
{
struct App
{
    App(std::string n);
    virtual ~App();

    HWND appHwnd;

  private:
    static auto CALLBACK WndProcCallback(HWND h, UINT m, WPARAM w, LPARAM l) -> LRESULT;
    virtual auto WndProc(HWND h, UINT m, WPARAM w, LPARAM l) -> LRESULT;

    auto OnClose(HWND h) -> int;
    auto OnDestroy() -> int;
};
} // namespace glow::gui
