// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <wil/resource.h>

#include <glow/message.hxx>
#include <glow/window.hxx>

namespace glow::app {
struct App {
    auto create() -> void;

private:
    static auto CALLBACK procedure(::HWND hwnd,
                                   ::UINT msg,
                                   ::WPARAM wparam,
                                   ::LPARAM lparam) -> ::LRESULT;

public:
    auto close() -> void;

    glow::window::Messages messages;
    wil::unique_hwnd hwnd;
};

auto run() -> int;
}; // namespace glow::app
