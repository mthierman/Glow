// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/message.hxx>

#include <stdlib.h>

#include <glow/system.hxx>

namespace glow::message {
auto Manager::on(::UINT msg, Callback callback) -> bool {
    auto emplace { map.try_emplace(msg, callback) };

    if (emplace.second) {
        return emplace.second;
    } else {
        auto insert { map.insert_or_assign(msg, callback) };

        return insert.second;
    }
}

auto Manager::contains(::UINT msg) -> bool { return map.contains(msg); }

auto Manager::invoke(glow::message::Message message) -> ::LRESULT {
    return map.find(message.msg)
        ->second({ message.hwnd, message.msg, message.wparam, message.lparam });
}

auto Manager::notify(Code code,
                     std::string_view message,
                     ::HWND senderHwnd,
                     uintptr_t senderId,
                     ::HWND receiverHwnd) -> void {
    Notification notification { .nmhdr { .hwndFrom { senderHwnd },
                                         .idFrom { senderId },
                                         .code { std::to_underlying(code) } },
                                .code { code },
                                .message { message } };
    send(receiverHwnd, WM_NOTIFY, senderId, &notification);
}

auto default_procedure(Message message) -> ::LRESULT {
    return ::DefWindowProcW(message.hwnd, message.msg, message.wparam, message.lparam);
}

auto run_loop() -> int {
    ::MSG msg {};
    int r {};

    while ((r = ::GetMessageW(&msg, nullptr, 0, 0)) != 0) {
        if (r == -1) {
            return EXIT_FAILURE;
        }

        else {
            ::TranslateMessage(&msg);
            ::DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

namespace wm {
    auto NOTIFY::nmhdr() -> const ::NMHDR& { return *reinterpret_cast<::NMHDR*>(lparam); }
    auto NOTIFY::notification() -> const Notification& {
        return *reinterpret_cast<Notification*>(lparam);
    }

    auto NCCREATE::createStruct() -> const ::CREATESTRUCTW& {
        return *reinterpret_cast<::CREATESTRUCTW*>(lparam);
    }

    auto DESTROY::quit(int exitCode) -> void { glow::system::quit(exitCode); }

    auto ACTIVATE::deactivated() -> bool { return LOWORD(wparam) == 0; }
    auto ACTIVATE::activated() -> bool { return LOWORD(wparam) == 1; }
    auto ACTIVATE::clickActivated() -> bool { return LOWORD(wparam) == 2; }
    auto ACTIVATE::hwnd() -> ::HWND { return reinterpret_cast<::HWND>(lparam); }

    auto ERASEBKGND::deviceContext() -> ::HDC { return reinterpret_cast<::HDC>(wparam); }

    auto KEYDOWN::virtualKeyCode() -> ::WORD { return LOWORD(wparam); }
    auto KEYDOWN::key() -> char { return static_cast<char>(virtualKeyCode()); }
    auto KEYDOWN::keyFlags() -> ::WORD { return HIWORD(lparam); }
    auto KEYDOWN::scanCode() -> ::BYTE { return LOBYTE(keyFlags()); }
    auto KEYDOWN::isExtendedKey() -> bool { return (keyFlags() & KF_EXTENDED) == KF_EXTENDED; }
    auto KEYDOWN::wasKeyDown() -> bool { return (keyFlags() & KF_REPEAT) == KF_REPEAT; }
    auto KEYDOWN::repeatCount() -> ::WORD { return LOWORD(lparam); }
    auto KEYDOWN::isKeyReleased() -> bool { return (keyFlags() & KF_UP) == KF_UP; }
    auto KEYDOWN::wasAltKeyDown() -> bool { return (keyFlags() & KF_ALTDOWN) == KF_ALTDOWN; }

    auto CHAR::characterCode() -> ::WORD { return LOWORD(wparam); }
    auto CHAR::character() -> char { return static_cast<char>(characterCode()); }
    auto CHAR::keyFlags() -> ::WORD { return HIWORD(lparam); }
    auto CHAR::scanCode() -> ::BYTE { return LOBYTE(keyFlags()); }
    auto CHAR::isExtendedKey() -> bool { return (keyFlags() & KF_EXTENDED) == KF_EXTENDED; }
    auto CHAR::wasKeyDown() -> bool { return (keyFlags() & KF_REPEAT) == KF_REPEAT; }
    auto CHAR::repeatCount() -> ::WORD { return LOWORD(lparam); }
    auto CHAR::isKeyReleased() -> bool { return (keyFlags() & KF_UP) == KF_UP; }
    auto CHAR::wasAltKeyDown() -> bool { return (keyFlags() & KF_ALTDOWN) == KF_ALTDOWN; }

    auto SHOWWINDOW::show() -> bool { return static_cast<bool>(wparam); }
    auto SHOWWINDOW::statusShow() -> bool { return lparam == 0; }
    auto SHOWWINDOW::parentClosing() -> bool { return lparam == 1; }
    auto SHOWWINDOW::otherZoom() -> bool { return lparam == 2; }
    auto SHOWWINDOW::parentOpening() -> bool { return lparam == 3; }
    auto SHOWWINDOW::otherUnzoom() -> bool { return lparam == 4; }

    auto SIZE::sizeRestored() -> bool { return wparam == 0; }
    auto SIZE::sizeMinimized() -> bool { return wparam == 1; }
    auto SIZE::sizeOtherRestored() -> bool { return wparam == 2; }
    auto SIZE::sizeMaximized() -> bool { return wparam == 3; }
    auto SIZE::sizeOtherMaximized() -> bool { return wparam == 4; }
    auto SIZE::size() -> ::SIZE { return ::SIZE { .cx = LOWORD(lparam), .cy = HIWORD(lparam) }; }

    auto DPICHANGED::y() -> ::WORD { return HIWORD(wparam); }
    auto DPICHANGED::x() -> ::WORD { return LOWORD(wparam); }
    auto DPICHANGED::suggestedRect() -> const ::RECT& { return *reinterpret_cast<::RECT*>(lparam); }
    auto DPICHANGED::userDefaultScreenDpi() -> int { return USER_DEFAULT_SCREEN_DPI; }

    auto GETMINMAXINFO::minMaxInfo() -> ::MINMAXINFO& {
        return *reinterpret_cast<::MINMAXINFO*>(lparam);
    }

    auto WINDOWPOSCHANGING::windowPos() -> ::WINDOWPOS& {
        return *reinterpret_cast<::WINDOWPOS*>(lparam);
    }
} // namespace wm
}; // namespace glow::message
