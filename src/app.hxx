// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <string>

#include <wil/resource.h>

#include "notification.hxx"
#include "random.hxx"

namespace glow
{
struct App
{
    App(std::string name = "App", size_t id = glow::random<size_t>());
    virtual ~App();

    virtual auto register_class() -> void;
    virtual auto operator()() -> int;
    virtual auto notify(::HWND receiver, CODE code, std::string message = "") -> void;
    virtual auto close() -> void;

    std::string m_name{};
    size_t m_id{};

    ::WNDCLASSEXA m_wcex{sizeof(::WNDCLASSEXA)};
    wil::unique_hwnd m_hwnd{};
    glow::Notification m_notification{};

  private:
    static auto CALLBACK StaticWndProc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT;
    virtual auto WndProc(::UINT message, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT;
    virtual auto on_close(::WPARAM wParam, ::LPARAM lParam) -> int;
    virtual auto on_destroy(::WPARAM wParam, ::LPARAM lParam) -> int;
};
} // namespace glow
