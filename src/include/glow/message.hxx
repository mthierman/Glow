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

enum struct Notice : ::UINT;

struct Notification {
    ::NMHDR nmhdr;
    ::HWND hwndFrom;
    ::UINT_PTR idFrom;
    ::UINT code;
    Notice notice;
    std::string message;
};

auto default_procedure(Message message) -> ::LRESULT;

struct NOTIFY : public Message {
    NOTIFY(const Message& message)
        : Message(message) { }
    auto nmhdr() -> const ::NMHDR&;
    auto notification() -> const Notification&;
};

struct NCCREATE : public Message {
    NCCREATE(const Message& message)
        : Message(message) { }
    auto createStruct() -> const ::CREATESTRUCTW&;
};

struct CREATE : public Message {
    CREATE(const Message& message)
        : Message(message) { }
    auto createStruct() -> const ::CREATESTRUCTW&;
};

struct ACTIVATE : public Message {
    ACTIVATE(const Message& message)
        : Message(message) { }
    auto deactivated() -> bool;
    auto activated() -> bool;
    auto clickActivated() -> bool;
    auto hwnd() -> ::HWND;
};

struct KEYDOWN : public Message {
    KEYDOWN(const Message& message)
        : Message(message) { }
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

struct KEYUP : public KEYDOWN {
    KEYUP(const Message& message)
        : KEYDOWN(message) { }
};

struct CHAR : public Message {
    CHAR(const Message& message)
        : Message(message) { }
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

struct SHOWWINDOW : public Message {
    SHOWWINDOW(const Message& message)
        : Message(message) { }
    auto show() -> bool;
    auto statusShow() -> bool;
    auto parentClosing() -> bool;
    auto otherZoom() -> bool;
    auto parentOpening() -> bool;
    auto otherUnzoom() -> bool;
};

struct SIZE : public Message {
    SIZE(const Message& message)
        : Message(message) { }
    auto sizeRestored() -> bool;
    auto sizeMinimized() -> bool;
    auto sizeOtherRestored() -> bool;
    auto sizeMaximized() -> bool;
    auto sizeOtherMaximized() -> bool;
    auto size() -> ::SIZE;
};

struct DPICHANGED : public Message {
    DPICHANGED(const Message& message)
        : Message(message) { }
    auto y() -> ::WORD;
    auto x() -> ::WORD;
    auto suggestedRect() -> const ::RECT&;
    auto userDefaultScreenDpi() -> int;
};

struct GETMINMAXINFO : public Message {
    GETMINMAXINFO(const Message& message)
        : Message(message) { }
    auto minMaxInfo() -> ::MINMAXINFO&;
};

struct WINDOWPOSCHANGED : public Message {
    WINDOWPOSCHANGED(const Message& message)
        : Message(message) { }
    auto windowPos() -> ::WINDOWPOS&;
};
}; // namespace glow::message
