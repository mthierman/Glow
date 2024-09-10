// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <functional>
#include <initializer_list>
#include <string>
#include <unordered_map>

namespace glow::message {
struct Message {
    ::HWND hwnd;
    ::UINT msg;
    ::WPARAM wparam;
    ::LPARAM lparam;
};

auto send(Message message) -> ::LRESULT;
auto post(Message message) -> ::LRESULT;

enum struct notice : ::UINT;

struct Notification {
    ::NMHDR nmhdr {
        .hwndFrom { nullptr },
        .idFrom { 0 },
        .code { 0 },
    };
    ::HWND hwndFrom { nullptr };
    ::UINT_PTR idFrom { 0 };
    ::UINT code { 0 };
    notice notice;
    std::string message;
};

struct wm : public Message {
    auto def_window_proc() -> ::LRESULT;
};

struct wm_notify : public wm {
    wm_notify(const wm& message)
        : wm(message) { }
    auto nmhdr() -> const ::NMHDR&;
    auto notification() -> const Notification&;
};

struct wm_nccreate : public wm {
    wm_nccreate(const wm& message)
        : wm(message) { }
    auto createStruct() -> const ::CREATESTRUCTW&;
};

struct wm_create : public wm {
    wm_create(const wm& message)
        : wm(message) { }
    auto createStruct() -> const ::CREATESTRUCTW&;
};

struct wm_activate : public wm {
    wm_activate(const wm& message)
        : wm(message) { }
    auto deactivated() -> bool;
    auto activated() -> bool;
    auto clickActivated() -> bool;
    auto hwnd() -> ::HWND;
};

struct wm_keydown_keyup : public wm {
    wm_keydown_keyup(const wm& message)
        : wm(message) { }
    auto virtualKeyCode() -> ::WORD;
    auto key() -> char;
    auto keyFlags() -> ::WORD;
    auto scanCode() -> ::BYTE;
    auto isExtendedKey() -> bool;
    auto wasKeyDown() -> bool;
    auto repeatCount() -> ::WORD;
    auto isKeyReleased() -> bool;
    auto wasAltKeyDown() -> bool;
};

struct wm_char : public wm {
    wm_char(const wm& message)
        : wm(message) { }
    auto characterCode() -> ::WORD;
    auto character() -> char;
    auto keyFlags() -> ::WORD;
    auto scanCode() -> ::BYTE;
    auto isExtendedKey() -> bool;
    auto wasKeyDown() -> bool;
    auto repeatCount() -> ::WORD;
    auto isKeyReleased() -> bool;
    auto wasAltKeyDown() -> bool;
};

struct wm_showwindow : public wm {
    wm_showwindow(const wm& message)
        : wm(message) { }
    auto show() -> bool;
    auto statusShow() -> bool;
    auto parentClosing() -> bool;
    auto otherZoom() -> bool;
    auto parentOpening() -> bool;
    auto otherUnzoom() -> bool;
};

struct wm_size : public wm {
    wm_size(const wm& message)
        : wm(message) { }
    auto sizeRestored() -> bool;
    auto sizeMinimized() -> bool;
    auto sizeOtherRestored() -> bool;
    auto sizeMaximized() -> bool;
    auto sizeOtherMaximized() -> bool;
    auto size() -> ::SIZE;
};

struct wm_dpichanged : public wm {
    wm_dpichanged(const wm& message)
        : wm(message) { }
    auto y() -> ::WORD;
    auto x() -> ::WORD;
    auto suggestedRect() -> const ::RECT&;
    auto userDefaultScreenDpi() -> int;
};

struct wm_getminmaxinfo : public wm {
    wm_getminmaxinfo(const wm& message)
        : wm(message) { }
    auto minMaxInfo() -> ::MINMAXINFO&;
};

struct wm_windowposchanged : public wm {
    wm_windowposchanged(const wm& message)
        : wm(message) { }
    auto windowPos() -> ::WINDOWPOS&;
};
}; // namespace glow::message
