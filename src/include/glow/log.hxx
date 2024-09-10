// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <format>
#include <string>

namespace glow::log {
auto format_message(::HRESULT errorCode) -> std::string;
auto get_last_error() -> std::string;

auto log(const std::string& message) -> void;
auto log(const std::wstring& message) -> void;

auto msg(const std::string& message) -> void;
auto msg(const std::wstring& message) -> void;

auto err(const std::string& message) -> void;
auto err(const std::wstring& message) -> void;

template <typename... Args>
auto log(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::OutputDebugStringA(std::vformat(fmt.get(), std::make_format_args(args...)).c_str());
    ::OutputDebugStringA("\n");
}

template <typename... Args>
auto log(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    ::OutputDebugStringA(
        glow::text::utf16_to_utf8(std::vformat(fmt.get(), std::make_wformat_args(args...)))
            .c_str());
    ::OutputDebugStringA("\n");
}

template <typename... Args>
auto msg(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxA(nullptr,
                  std::vformat(fmt.get(), std::make_format_args(args...)).c_str(),
                  nullptr,
                  MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto msg(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxA(
        nullptr,
        glow::text::utf16_to_utf8(std::vformat(fmt.get(), std::make_wformat_args(args...))).c_str(),
        nullptr,
        MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto err(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxA(nullptr,
                  std::vformat(fmt.get(), std::make_format_args(args...)).c_str(),
                  nullptr,
                  MB_OK | MB_ICONHAND);
}

template <typename... Args>
auto err(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxW(
        nullptr,
        glow::text::utf16_to_utf8(std::vformat(fmt.get(), std::make_wformat_args(args...))).c_str(),
        nullptr,
        MB_OK | MB_ICONHAND);
}
}; // namespace glow::log
