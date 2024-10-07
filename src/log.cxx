// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/log.hxx>

namespace glow::log {
auto log(std::u8string_view message) -> void {
    auto converted { glow::text::u16string(message) };

    if (converted.has_value()) {
        ::OutputDebugStringW(reinterpret_cast<const wchar_t*>(converted.value().data()));
        ::OutputDebugStringW(L"\n");
    }
}

auto log(::HRESULT errorCode) -> void {
    auto converted { glow::text::u16string(format_message(errorCode)) };

    if (converted.has_value()) {
        ::OutputDebugStringW(reinterpret_cast<const wchar_t*>(converted.value().data()));
        ::OutputDebugStringW(L"\n");
    }
}

auto message(std::u8string_view message) -> void {
    auto converted { glow::text::u16string(message) };

    if (converted.has_value()) {
        ::MessageBoxW(nullptr,
                      reinterpret_cast<wchar_t*>(converted.value().data()),
                      nullptr,
                      MB_OK | MB_ICONASTERISK);
    }
}

auto error(std::u8string_view message) -> void {
    auto converted { glow::text::u16string(message) };

    if (converted.has_value()) {
        ::MessageBoxW(nullptr,
                      reinterpret_cast<wchar_t*>(converted.value().data()),
                      nullptr,
                      MB_OK | MB_ICONHAND);
    }
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

    if (auto converted { glow::text::u8string(reinterpret_cast<char16_t*>(buffer.get())) };
        converted.has_value()) {
        return converted.value();
    }

    return {};
}

auto get_last_error() -> std::u8string { return format_message(::GetLastError()); }
}; // namespace glow::log
