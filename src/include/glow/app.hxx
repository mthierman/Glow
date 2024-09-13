// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <wil/resource.h>

#include <glow/math.hxx>
#include <glow/message.hxx>

namespace glow::app {
struct App {
    auto create() -> void;

private:
    static auto CALLBACK procedure(::HWND hwnd,
                                   ::UINT msg,
                                   ::WPARAM wparam,
                                   ::LPARAM lparam) -> ::LRESULT;

public:
    auto notify_app(glow::message::Code code,
                    std::string_view message = "",
                    ::HWND receiverHwnd
                    = ::FindWindowExW(HWND_MESSAGE, nullptr, L"App", nullptr)) -> void;

    uintptr_t id { glow::math::make_random<uintptr_t>() };
    glow::message::Manager messages;

    wil::unique_hwnd hwnd;
};

auto run() -> int;
}; // namespace glow::app
