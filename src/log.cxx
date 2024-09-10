// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/log.hxx>

#include <stdexcept>

#include <wil/resource.h>

namespace glow::log {
auto format_message(::HRESULT errorCode) -> std::string {
    wil::unique_hlocal_ansistring buffer;

    if (::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                             | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
                         nullptr,
                         errorCode,
                         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                         wil::out_param_ptr<::LPSTR>(buffer),
                         0,
                         nullptr)
        == 0) {
        throw std::runtime_error(get_last_error());
    }

    return buffer.get();
}

auto get_last_error() -> std::string { return format_message(::GetLastError()); }

auto log(const std::string& message) -> void {
    ::OutputDebugStringA(message.c_str());
    ::OutputDebugStringA("\n");
}

auto log(const std::wstring& message) -> void {
    ::OutputDebugStringA(glow::text::utf16_to_utf8(message).c_str());
    ::OutputDebugStringA("\n");
}

auto msg(const std::string& message) -> void {
    ::MessageBoxA(nullptr, message.c_str(), nullptr, MB_OK | MB_ICONASTERISK);
}

auto msg(const std::wstring& message) -> void {
    ::MessageBoxA(
        nullptr, glow::text::utf16_to_utf8(message).c_str(), nullptr, MB_OK | MB_ICONASTERISK);
}

auto err(const std::string& message) -> void {
    ::MessageBoxA(nullptr, message.c_str(), nullptr, MB_OK | MB_ICONHAND);
}

auto err(const std::wstring& message) -> void {
    ::MessageBoxA(
        nullptr, glow::text::utf16_to_utf8(message).c_str(), nullptr, MB_OK | MB_ICONHAND);
}
}; // namespace glow::log
