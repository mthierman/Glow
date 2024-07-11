// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <glow/std.hxx>

#include <Windows.h>

#include <ShlObj.h>
#include <gdiplus.h>

#include <stdlib.h>

#include <format>
#include <functional>
#include <print>
#include <string>

#include <wil/resource.h>

namespace glow::system {
auto co_initialize(::DWORD coInit = COINIT_APARTMENTTHREADED
                       | COINIT_DISABLE_OLE1DDE) -> wil::unique_couninitialize_call;
auto gdi_plus_startup() -> ::ULONG_PTR;
auto gdi_plus_shutdown(::ULONG_PTR token) -> void;
auto create_process(std::filesystem::path path) -> int;
auto get_instance() -> ::HMODULE;
auto abort(::UINT exitCode = EXIT_FAILURE) -> void;
auto quit(int exitCode = EXIT_SUCCESS) -> void;
auto load_system_brush(int name = BLACK_BRUSH) -> ::HBRUSH;
auto load_system_cursor(LPSTR name = IDC_ARROW) -> ::HCURSOR;
auto load_system_icon(LPSTR name = IDI_APPLICATION) -> ::HICON;
auto load_resource_icon() -> ::HICON;
auto make_guid() -> ::GUID;
auto format_message(::HRESULT errorCode) -> std::string;
auto get_last_error() -> std::string;

auto dbg(const std::string& message) -> void;
auto dbg(const std::wstring& message) -> void;

template <typename... Args>
auto dbg(const std::format_string<Args...> fmt, Args&&... args) -> void {
    ::OutputDebugStringA(std::vformat(fmt.get(), std::make_format_args(args...)).c_str());
    ::OutputDebugStringA("\n");
}

template <typename... Args>
auto dbg(const std::wformat_string<Args...> fmt, Args&&... args) -> void {
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

struct Event {
    auto create(const std::string& name, std::function<void()> callback = []() {}) -> bool;

    wil::unique_event_nothrow event;
    wil::unique_event_watcher_nothrow watcher;
    std::function<void()> m_callback { []() {} };
};
}; // namespace glow::system
