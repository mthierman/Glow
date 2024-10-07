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
    ::OutputDebugStringW(glow::text::to_wstring(format_message(errorCode)).c_str());
    ::OutputDebugStringW(L"\n");
}

auto message(std::u8string_view message) -> void {
    ::MessageBoxW(
        nullptr, glow::text::to_wstring(message).c_str(), nullptr, MB_OK | MB_ICONASTERISK);
}

auto error(std::u8string_view message) -> void {
    ::MessageBoxW(nullptr, glow::text::to_wstring(message).c_str(), nullptr, MB_OK | MB_ICONHAND);
}

auto format_message(::HRESULT errorCode) -> std::u8string {
    wil::unique_hlocal_string buffer;

    ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                         | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
                     nullptr,
                     errorCode,
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                     wil::out_param_ptr<::LPWSTR>(buffer),
                     0,
                     nullptr);

    return glow::text::to_u8string(buffer.get());
}

auto get_last_error() -> std::u8string { return format_message(::GetLastError()); }
}; // namespace glow::log
