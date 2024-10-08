// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/log.hxx>

#include <source_location>

namespace glow::log {
auto log(std::u8string_view message) -> void {
    if (auto functionName {
            glow::text::u16string(std::source_location::current().function_name()) };
        functionName.has_value()) {
        ::OutputDebugStringW(glow::text::c_str(functionName.value()));
        ::OutputDebugStringW(L": ");
    }

    if (auto convertedMessage { glow::text::u16string(message) }; convertedMessage.has_value()) {
        ::OutputDebugStringW(glow::text::c_str(convertedMessage.value()));
    }

    ::OutputDebugStringW(L"\n");
}

auto log(::HRESULT errorCode) -> void {
    if (auto functionName {
            glow::text::u16string(std::source_location::current().function_name()) };
        functionName.has_value()) {
        ::OutputDebugStringW(glow::text::c_str(functionName.value()));
        ::OutputDebugStringW(L": ");
    }

    if (auto formattedMessage { glow::text::u16string(format_message(errorCode)) };
        formattedMessage.has_value()) {
        ::OutputDebugStringW(glow::text::c_str(formattedMessage.value()));
    }

    ::OutputDebugStringW(L"\n");
}

auto message(std::u8string_view message) -> void {
    if (auto converted { glow::text::u16string(message) }; converted.has_value()) {
        ::MessageBoxW(
            nullptr, glow::text::c_str(converted.value()), nullptr, MB_OK | MB_ICONASTERISK);
    }
}

auto error(std::u8string_view message) -> void {
    if (auto converted { glow::text::u16string(message) }; converted.has_value()) {
        ::MessageBoxW(nullptr, glow::text::c_str(converted.value()), nullptr, MB_OK | MB_ICONHAND);
    }
}

auto format_message(::HRESULT errorCode) -> std::u8string {
    wil::unique_hlocal_string buffer;

    ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                         | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
                     nullptr,
                     errorCode,
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                     wil::out_param_ptr<wchar_t*>(buffer),
                     0,
                     nullptr);

    if (auto converted { glow::text::u8string(buffer.get()) }; converted.has_value()) {
        return converted.value();
    }

    return {};
}

auto get_last_error() -> std::u8string { return format_message(::GetLastError()); }
}; // namespace glow::log
