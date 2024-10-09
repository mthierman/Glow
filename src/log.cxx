// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/log.hxx>

namespace glow::log {
auto log(std::u8string_view message, const std::source_location& location) -> void {
    if (auto lineNumber { glow::text::u16string(std::format("{}", location.line())) }; lineNumber) {
        ::OutputDebugStringW(glow::text::c_str(lineNumber.value()));
        ::OutputDebugStringW(L" - ");
    }

    if (auto functionName { glow::text::u16string(location.function_name()) }; functionName) {
        ::OutputDebugStringW(glow::text::c_str(functionName.value()));
        ::OutputDebugStringW(L": ");
    }

    if (auto convertedMessage { glow::text::u16string(message) }; convertedMessage) {
        ::OutputDebugStringW(glow::text::c_str(convertedMessage.value()));
    }

    ::OutputDebugStringW(L"\n");
}

auto log(::HRESULT errorCode, const std::source_location& location) -> void {
    if (auto lineNumber { glow::text::u16string(std::format("{}", location.line())) }; lineNumber) {
        ::OutputDebugStringW(glow::text::c_str(lineNumber.value()));
        ::OutputDebugStringW(L" - ");
    }

    if (auto functionName { glow::text::u16string(location.function_name()) }; functionName) {
        ::OutputDebugStringW(glow::text::c_str(functionName.value()));
        ::OutputDebugStringW(L": ");
    }

    if (auto formattedMessage { glow::text::u16string(format_message(errorCode)) };
        formattedMessage) {
        ::OutputDebugStringW(glow::text::c_str(formattedMessage.value()));
    }

    ::OutputDebugStringW(L"\n");
}

auto message(std::u8string_view message) -> void {
    if (auto converted { glow::text::u16string(message) }; converted) {
        ::MessageBoxW(
            nullptr, glow::text::c_str(converted.value()), nullptr, MB_OK | MB_ICONASTERISK);
    }
}

auto error(std::u8string_view message) -> void {
    if (auto converted { glow::text::u16string(message) }; converted) {
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

    if (auto converted { glow::text::u8string(buffer.get()) }; converted) {
        return converted.value();
    }

    return {};
}

auto get_last_error() -> std::u8string { return format_message(::GetLastError()); }
}; // namespace glow::log
