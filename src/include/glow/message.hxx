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

// enum struct Notice : ::UINT;

// struct Notification {
//     ::NMHDR nmhdr;
//     ::HWND hwndFrom;
//     ::UINT_PTR idFrom;
//     ::UINT code;
//     Notice notice;
//     std::string message;
// };

auto default_procedure(Message message) -> ::LRESULT;

struct NOTIFY : public Message {
    wm_notify(const wm& message)
        : wm(message) { }
    auto nmhdr() -> const ::NMHDR&;
    auto notification() -> const Notification&;
};

struct NCCREATE : public Message {
    wm_nccreate(const wm& message)
        : wm(message) { }
    auto createStruct() -> const ::CREATESTRUCTW&;
};

struct wm_create : public Message {
    wm_create(const wm& message)
        : wm(message) { }
    auto createStruct() -> const ::CREATESTRUCTW&;
};

struct wm_activate : public Message {
    wm_activate(const wm& message)
        : wm(message) { }
    auto deactivated() -> bool;
    auto activated() -> bool;
    auto clickActivated() -> bool;
    auto hwnd() -> ::HWND;
};

struct wm_keydown_keyup : public Message {
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

struct wm_char : public Message {
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

struct wm_showwindow : public Message {
    wm_showwindow(const wm& message)
        : wm(message) { }
    auto show() -> bool;
    auto statusShow() -> bool;
    auto parentClosing() -> bool;
    auto otherZoom() -> bool;
    auto parentOpening() -> bool;
    auto otherUnzoom() -> bool;
};

struct wm_size : public Message {
    wm_size(const wm& message)
        : wm(message) { }
    auto sizeRestored() -> bool;
    auto sizeMinimized() -> bool;
    auto sizeOtherRestored() -> bool;
    auto sizeMaximized() -> bool;
    auto sizeOtherMaximized() -> bool;
    auto size() -> ::SIZE;
};

struct wm_dpichanged : public Message {
    wm_dpichanged(const wm& message)
        : wm(message) { }
    auto y() -> ::WORD;
    auto x() -> ::WORD;
    auto suggestedRect() -> const ::RECT&;
    auto userDefaultScreenDpi() -> int;
};

struct wm_getminmaxinfo : public Message {
    wm_getminmaxinfo(const wm& message)
        : wm(message) { }
    auto minMaxInfo() -> ::MINMAXINFO&;
};

struct wm_windowposchanged : public Message {
    wm_windowposchanged(const wm& message)
        : wm(message) { }
    auto windowPos() -> ::WINDOWPOS&;
};
}; // namespace glow::message
