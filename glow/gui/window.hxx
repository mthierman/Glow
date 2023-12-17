// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <bit>

#include <gui/gui.hxx>

namespace glow::gui
{

struct Window
{
    Window();
    virtual ~Window();
    auto register_class() -> ATOM;
    auto create() -> HWND;
    auto show() -> void;

    ATOM m_atom;
    wil::unique_hwnd m_hwnd;
    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto on_close() -> int;
    virtual auto on_size() -> int;
};

} // namespace glow::gui
