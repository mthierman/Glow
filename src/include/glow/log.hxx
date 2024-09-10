// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <format>
#include <string>

#include <wil/resource.h>

namespace glow::log {
auto format_message(::HRESULT errorCode) -> std::string;
auto get_last_error() -> std::string;

auto log(const std::string& message) -> void;
auto log(const std::wstring& message) -> void;

template <typename... Args>
auto log(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::OutputDebugStringA(std::vformat(fmt.get(), std::make_format_args(args...)).c_str());
    ::OutputDebugStringA("\n");
}

template <typename... Args>
auto log(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    ::OutputDebugStringW(std::vformat(fmt.get(), std::make_wformat_args(args...)).c_str());
    ::OutputDebugStringW(L"\n");
}

template <typename... Args>
auto message_box(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxA(nullptr,
                  std::vformat(fmt.get(), std::make_format_args(args...)).c_str(),
                  nullptr,
                  MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto message_box(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxW(nullptr,
                  std::vformat(fmt.get(), std::make_wformat_args(args...)).c_str(),
                  nullptr,
                  MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto error_box(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxA(nullptr,
                  std::vformat(fmt.get(), std::make_format_args(args...)).c_str(),
                  nullptr,
                  MB_OK | MB_ICONHAND);
}

template <typename... Args>
auto error_box(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxW(nullptr,
                  std::vformat(fmt.get(), std::make_wformat_args(args...)).c_str(),
                  nullptr,
                  MB_OK | MB_ICONHAND);
}
}; // namespace glow::log
