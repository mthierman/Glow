// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <functional>
#include <string>
#include <unordered_map>

namespace glow::message {
struct Message {
    ::HWND hwnd;
    ::UINT msg;
    ::WPARAM wparam;
    ::LPARAM lparam;
};

enum struct Code : ::UINT;

struct Notification {
    ::NMHDR nmhdr;
    Code code;
    std::string_view message;
};

struct Manager {
    using Callback = std::function<::LRESULT(glow::message::Message)>;

    auto on(::UINT msg, Callback callback) -> bool;
    auto contains(::UINT msg) -> bool;
    auto invoke(glow::message::Message message) -> ::LRESULT;

    template <typename W, typename L>
    auto send(::HWND hwnd, ::UINT msg, W wparam, L lparam) -> ::LRESULT {
        return ::SendMessageW(hwnd, msg, (::WPARAM)wparam, (::LPARAM)lparam);
    }

    template <typename W, typename L>
    auto post(::HWND hwnd, ::UINT msg, W wparam, L lparam) -> ::LRESULT {
        return ::PostMessageW(hwnd, msg, (::WPARAM)wparam, (::LPARAM)lparam);
    }

    auto notify(Code code,
                std::string_view message,
                ::HWND senderHwnd,
                uintptr_t senderId,
                ::HWND receiverHwnd) -> void;

private:
    std::unordered_map<::UINT, Callback> map;
};

auto default_procedure(Message message) -> ::LRESULT;

namespace wm {
    struct MSG : public Message {
        MSG(const Message& message)
            : Message(message) { }
    };

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

    struct CREATE : public NCCREATE {
        CREATE(const Message& message)
            : NCCREATE(message) { }
    };

    struct DESTROY : public Message {
        DESTROY(const Message& message)
            : Message(message) { }
        auto quit(int exitCode = 0) -> void;
    };

    struct ACTIVATE : public Message {
        ACTIVATE(const Message& message)
            : Message(message) { }
        auto deactivated() -> bool;
        auto activated() -> bool;
        auto clickActivated() -> bool;
        auto hwnd() -> ::HWND;
    };

    struct ERASEBKGND : public Message {
        ERASEBKGND(const Message& message)
            : Message(message) { }
        auto deviceContext() -> ::HDC;
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
} // namespace wm
}; // namespace glow::message
