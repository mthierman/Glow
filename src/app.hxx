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
    App(std::string name = "App", size_t id = glow::random<size_t>());
    virtual ~App();

    virtual auto operator()() -> int;

    size_t m_id{};
    wil::unique_hwnd m_hwnd{};

  private:
    static auto CALLBACK WndProc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT;
    virtual auto default_wnd_proc(::HWND hwnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT;
    virtual auto wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT;
    virtual auto on_close(::WPARAM wParam, ::LPARAM lParam) -> int;
    virtual auto on_destroy(::WPARAM wParam, ::LPARAM lParam) -> int;
};
} // namespace glow
