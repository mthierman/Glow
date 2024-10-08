// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <format>
#include <source_location>
#include <string>

#include <wil/resource.h>

#include <glow/text.hxx>

namespace glow::log {
auto log(std::u8string_view message) -> void;
auto log(::HRESULT errorCode) -> void;
auto message(std::u8string_view message) -> void;
auto error(std::u8string_view message) -> void;
auto format_message(::HRESULT errorCode) -> std::u8string;
auto get_last_error() -> std::u8string;

template <typename... Args>
auto log(const std::format_string<Args...> fmt, Args&&... args) -> void {
    if (auto functionName {
            glow::text::u16string(std::source_location::current().function_name()) };
        functionName.has_value()) {
        ::OutputDebugStringW(glow::text::c_str(functionName.value()));
        ::OutputDebugStringW(L": ");
    }

    if (auto converted {
            glow::text::u16string(std::vformat(fmt.get(), std::make_format_args(args...))) };
        converted.has_value()) {
        ::OutputDebugStringW(glow::text::c_str(converted.value()));
    }

    ::OutputDebugStringW(L"\n");
}

template <typename... Args>
auto log(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    if (auto functionName {
            glow::text::u16string(std::source_location::current().function_name()) };
        functionName.has_value()) {
        ::OutputDebugStringW(glow::text::c_str(functionName.value()));
        ::OutputDebugStringW(L": ");
    }

    ::OutputDebugStringW(std::vformat(fmt.get(), std::make_wformat_args(args...)).c_str());
    ::OutputDebugStringW(L"\n");
}

template <typename... Args>
auto message(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxW(nullptr,
                  glow::text::c_str(glow::text::u16string(
                      std::vformat(fmt.get(), std::make_format_args(args...)))),
                  nullptr,
                  MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto message(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxW(nullptr,
                  std::vformat(fmt.get(), std::make_wformat_args(args...)).c_str(),
                  nullptr,
                  MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto error(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxW(nullptr,
                  glow::text::c_str(glow::text::u16string(
                      std::vformat(fmt.get(), std::make_format_args(args...)))),
                  nullptr,
                  MB_OK | MB_ICONHAND);
}

template <typename... Args>
auto error(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxW(nullptr,
                  std::vformat(fmt.get(), std::make_wformat_args(args...)).c_str(),
                  nullptr,
                  MB_OK | MB_ICONHAND);
}
}; // namespace glow::log
