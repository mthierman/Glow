// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/log.hxx>

#include <glow/system.hxx>

namespace glow::log {
auto log(std::u8string_view message, const std::source_location& location) -> void {
    if (auto line { glow::text::u16string(std::format("[{}] ", location.line())) }) {
        ::OutputDebugStringW(glow::text::c_str(*line));
    }

    if (auto functionName { glow::text::u16string(location.function_name()) }) {
        ::OutputDebugStringW(glow::text::c_str(*functionName));
        ::OutputDebugStringW(L": ");
    }

    if (auto converted { glow::text::u16string(message) }) {
        ::OutputDebugStringW(glow::text::c_str(*converted));
    }

    ::OutputDebugStringW(L"\n");
}

auto log(::HRESULT errorCode, const std::source_location& location) -> void {
    if (auto line { glow::text::u16string(std::format("[{}] ", location.line())) }) {
        ::OutputDebugStringW(glow::text::c_str(*line));
    }

    if (auto functionName { glow::text::u16string(location.function_name()) }) {
        ::OutputDebugStringW(glow::text::c_str(*functionName));
        ::OutputDebugStringW(L": ");
    }

    if (auto converted { glow::text::u16string(glow::system::format_message(errorCode)) }) {
        ::OutputDebugStringW(glow::text::c_str(*converted));
    }

    ::OutputDebugStringW(L"\n");
}

auto message(std::u8string_view message) -> void {
    if (auto converted { glow::text::u16string(message) }) {
        ::MessageBoxW(nullptr, glow::text::c_str(*converted), nullptr, MB_OK | MB_ICONASTERISK);
    }
}

auto error(std::u8string_view message) -> void {
    if (auto converted { glow::text::u16string(message) }) {
        ::MessageBoxW(nullptr, glow::text::c_str(*converted), nullptr, MB_OK | MB_ICONHAND);
    }
}
}; // namespace glow::log
