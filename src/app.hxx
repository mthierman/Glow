// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <stdexcept>
#include <string>

#include <wil/resource.h>

#include "gui.hxx"
#include "random.hxx"

namespace glow
{
struct App
{
    App(std::string name = "App", intptr_t id = glow::random<intptr_t>());

    virtual ~App();

    virtual auto operator()() -> int;

    virtual auto hwnd() const -> HWND;

    virtual auto id() const -> intptr_t;

    virtual auto close() -> int;

    virtual auto quit() -> int;

  private:
    static auto CALLBACK WndProc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT;
    virtual auto default_wnd_proc(::HWND hwnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT;
    virtual auto wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT;

  public:
    intptr_t m_id{};
    wil::unique_hwnd m_hwnd{};
};
} // namespace glow
