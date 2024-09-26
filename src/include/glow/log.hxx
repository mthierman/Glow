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

#include <glow/text.hxx>

namespace glow::log {
auto get_last_error() -> std::string;

auto log(const std::string& message) -> void;
auto log(const std::wstring& message) -> void;
auto log(::HRESULT errorCode) -> void;

auto msg(const std::string& message) -> void;
auto msg(const std::wstring& message) -> void;

auto err(const std::string& message) -> void;
auto err(const std::wstring& message) -> void;

template <typename... Args>
auto log(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::OutputDebugStringW(
        glow::text::to_wstring(std::vformat(fmt.get(), std::make_format_args(args...))).c_str());
    ::OutputDebugStringW(L"\n");
}

template <typename... Args>
auto log(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    ::OutputDebugStringW(std::vformat(fmt.get(), std::make_wformat_args(args...)).c_str());
    ::OutputDebugStringW(L"\n");
}

template <typename... Args>
auto msg(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxW(
        nullptr,
        glow::text::to_wstring(std::vformat(fmt.get(), std::make_format_args(args...))).c_str(),
        nullptr,
        MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto msg(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxW(nullptr,
                  std::vformat(fmt.get(), std::make_wformat_args(args...)).c_str(),
                  nullptr,
                  MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto err(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxW(
        nullptr,
        glow::text::to_wstring(std::vformat(fmt.get(), std::make_format_args(args...))).c_str(),
        nullptr,
        MB_OK | MB_ICONHAND);
}

template <typename... Args>
auto err(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
    ::MessageBoxW(nullptr,
                  std::vformat(fmt.get(), std::make_wformat_args(args...)).c_str(),
                  nullptr,
                  MB_OK | MB_ICONHAND);
}

template <typename T> auto format_message(::HRESULT errorCode) -> T {
    wil::unique_hlocal_string buffer;

    ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                         | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
                     nullptr,
                     errorCode,
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                     wil::out_param_ptr<::LPWSTR>(buffer),
                     0,
                     nullptr);

    //  if constexpr (std::is_same_v())
    if constexpr (std::is_same_v<T, std::string>) {
        return glow::text::to_string(buffer.get());
    }

    if constexpr (std::is_same_v<T, std::wstring>) {
        return buffer.get();
    }

    // return glow::text::to_string(buffer.get());
}
}; // namespace glow::log
