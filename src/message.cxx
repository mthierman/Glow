// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/message.hxx>

namespace glow::message {
auto MessageHandler::on(::UINT msg, MessageCallback callback) -> bool {
    auto emplace { map.try_emplace(msg, callback) };

    return emplace.second;
}

auto MessageHandler::contains(::UINT msg) -> bool { return map.contains(msg); }

auto MessageHandler::invoke(Message message) -> ::LRESULT {
    return map.find(message.msg)
        ->second({ message.hwnd, message.msg, message.wparam, message.lparam });
}

auto wm::def_window_proc() -> ::LRESULT { return ::DefWindowProcA(hwnd, msg, wparam, lparam); }

auto wm_notify::nmhdr() -> const ::NMHDR& { return *reinterpret_cast<::NMHDR*>(lparam); }
auto wm_notify::notification() -> const Notification& {
    return *reinterpret_cast<Notification*>(lparam);
}

auto wm_nccreate::createStruct() -> const ::CREATESTRUCTW& {
    return *reinterpret_cast<::CREATESTRUCTW*>(lparam);
}

auto wm_create::createStruct() -> const ::CREATESTRUCTW& {
    return *reinterpret_cast<::CREATESTRUCTW*>(lparam);
}

auto wm_activate::deactivated() -> bool { return LOWORD(wparam) == 0; }
auto wm_activate::activated() -> bool { return LOWORD(wparam) == 1; }
auto wm_activate::clickActivated() -> bool { return LOWORD(wparam) == 2; }
auto wm_activate::hwnd() -> ::HWND { return reinterpret_cast<::HWND>(lparam); }

auto wm_keydown_keyup::virtualKeyCode() -> ::WORD { return LOWORD(wparam); }
auto wm_keydown_keyup::key() -> char { return static_cast<char>(virtualKeyCode()); }
auto wm_keydown_keyup::keyFlags() -> ::WORD { return HIWORD(lparam); }
auto wm_keydown_keyup::scanCode() -> ::BYTE { return LOBYTE(keyFlags()); }
auto wm_keydown_keyup::isExtendedKey() -> bool { return (keyFlags() & KF_EXTENDED) == KF_EXTENDED; }
auto wm_keydown_keyup::wasKeyDown() -> bool { return (keyFlags() & KF_REPEAT) == KF_REPEAT; }
auto wm_keydown_keyup::repeatCount() -> ::WORD { return LOWORD(lparam); }
auto wm_keydown_keyup::isKeyReleased() -> bool { return (keyFlags() & KF_UP) == KF_UP; }
auto wm_keydown_keyup::wasAltKeyDown() -> bool { return (keyFlags() & KF_ALTDOWN) == KF_ALTDOWN; }

auto wm_char::characterCode() -> ::WORD { return LOWORD(wparam); }
auto wm_char::character() -> char { return static_cast<char>(characterCode()); }
auto wm_char::keyFlags() -> ::WORD { return HIWORD(lparam); }
auto wm_char::scanCode() -> ::BYTE { return LOBYTE(keyFlags()); }
auto wm_char::isExtendedKey() -> bool { return (keyFlags() & KF_EXTENDED) == KF_EXTENDED; }
auto wm_char::wasKeyDown() -> bool { return (keyFlags() & KF_REPEAT) == KF_REPEAT; }
auto wm_char::repeatCount() -> ::WORD { return LOWORD(lparam); }
auto wm_char::isKeyReleased() -> bool { return (keyFlags() & KF_UP) == KF_UP; }
auto wm_char::wasAltKeyDown() -> bool { return (keyFlags() & KF_ALTDOWN) == KF_ALTDOWN; }

auto wm_showwindow::show() -> bool { return static_cast<bool>(wparam); }
auto wm_showwindow::statusShow() -> bool { return lparam == 0; }
auto wm_showwindow::parentClosing() -> bool { return lparam == 1; }
auto wm_showwindow::otherZoom() -> bool { return lparam == 2; }
auto wm_showwindow::parentOpening() -> bool { return lparam == 3; }
auto wm_showwindow::otherUnzoom() -> bool { return lparam == 4; }

auto wm_size::sizeRestored() -> bool { return wparam == 0; }
auto wm_size::sizeMinimized() -> bool { return wparam == 1; }
auto wm_size::sizeOtherRestored() -> bool { return wparam == 2; }
auto wm_size::sizeMaximized() -> bool { return wparam == 3; }
auto wm_size::sizeOtherMaximized() -> bool { return wparam == 4; }
auto wm_size::size() -> ::SIZE { return ::SIZE { .cx = LOWORD(lparam), .cy = HIWORD(lparam) }; }

auto wm_dpichanged::y() -> ::WORD { return HIWORD(wparam); }
auto wm_dpichanged::x() -> ::WORD { return LOWORD(wparam); }
auto wm_dpichanged::suggestedRect() -> const ::RECT& { return *reinterpret_cast<::RECT*>(lparam); }
auto wm_dpichanged::userDefaultScreenDpi() -> int { return USER_DEFAULT_SCREEN_DPI; }

auto wm_getminmaxinfo::minMaxInfo() -> ::MINMAXINFO& {
    return *reinterpret_cast<::MINMAXINFO*>(lparam);
}

auto wm_windowposchanged::windowPos() -> ::WINDOWPOS& {
    return *reinterpret_cast<::WINDOWPOS*>(lparam);
}
}; // namespace glow::message
