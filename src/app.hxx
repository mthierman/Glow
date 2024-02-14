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

    intptr_t m_id{};
    wil::unique_hwnd m_hwnd{};

  private:
    static auto CALLBACK WndProc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT;
    virtual auto default_wnd_proc(::HWND hwnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT;
    virtual auto wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT;
};
} // namespace glow
