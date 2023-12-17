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
#include <initializer_list>
#include <print>

#include <gui/gui.hxx>

namespace glow::gui
{

struct Window
{
    Window();
    Window(std::string title);
    virtual ~Window();

    auto set_title(std::string title) -> void;
    auto show() -> void;
    auto hide() -> void;

  private:
    auto register_class() -> ATOM;
    auto create_window() -> HWND;
    auto create() -> void;
    auto show_normal() -> void;

    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto on_close() -> int;

  public:
    std::string m_title{"Window"};
    ATOM m_atom;
    wil::unique_hwnd m_hwnd;
};

} // namespace glow::gui
