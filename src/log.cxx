// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/log.hxx>

namespace glow::log {
auto log(std::u8string_view message) -> void {
    ::OutputDebugStringW(glow::text::to_wstring(message).c_str());
    ::OutputDebugStringW(L"\n");
}

auto log(::HRESULT errorCode) -> void {
    ::OutputDebugStringW(format_message<std::wstring>(errorCode).c_str());
    ::OutputDebugStringW(L"\n");
}

auto message(std::u8string_view message) -> void {
    ::MessageBoxW(
        nullptr, glow::text::to_wstring(message).c_str(), nullptr, MB_OK | MB_ICONASTERISK);
}

auto error(std::u8string_view message) -> void {
    ::MessageBoxW(nullptr, glow::text::to_wstring(message).c_str(), nullptr, MB_OK | MB_ICONHAND);
}
}; // namespace glow::log
