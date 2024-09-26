// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/log.hxx>

namespace glow::log {

auto log(const std::string& message) -> void {
    ::OutputDebugStringW(glow::text::to_wstring(message).c_str());
    ::OutputDebugStringW(L"\n");
}

auto log(const std::wstring& message) -> void {
    ::OutputDebugStringW(message.c_str());
    ::OutputDebugStringW(L"\n");
}

auto log(::HRESULT errorCode) -> void {
    ::OutputDebugStringW(format_message<std::wstring>(errorCode).c_str());
    ::OutputDebugStringW(L"\n");
}

auto msg(const std::string& message) -> void {
    ::MessageBoxW(
        nullptr, glow::text::to_wstring(message).c_str(), nullptr, MB_OK | MB_ICONASTERISK);
}

auto msg(const std::wstring& message) -> void {
    ::MessageBoxW(nullptr, message.c_str(), nullptr, MB_OK | MB_ICONASTERISK);
}

auto err(const std::string& message) -> void {
    ::MessageBoxW(nullptr, glow::text::to_wstring(message).c_str(), nullptr, MB_OK | MB_ICONHAND);
}

auto err(const std::wstring& message) -> void {
    ::MessageBoxW(nullptr, message.c_str(), nullptr, MB_OK | MB_ICONHAND);
}
}; // namespace glow::log
