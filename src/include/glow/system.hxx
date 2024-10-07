// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <ShlObj.h>
#include <gdiplus.h>

#include <stdlib.h>

#include <filesystem>
#include <functional>
#include <initializer_list>
#include <string>
#include <vector>

#include <wil/resource.h>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.UI.ViewManagement.h>

namespace winrt {
using namespace winrt::Windows::UI::ViewManagement;
}; // namespace winrt

namespace glow::system {
auto co_initialize(::COINIT coInit = ::COINIT::COINIT_APARTMENTTHREADED)
    -> wil::unique_couninitialize_call;
auto create_process(const std::filesystem::path& path, std::u8string_view commandLine) -> int;
auto instance() -> ::HMODULE;
auto exit_process(unsigned int exitCode = EXIT_SUCCESS) -> void;
auto quit(int exitCode = EXIT_SUCCESS) -> void;
auto system_brush(int name = NULL_BRUSH) -> ::HBRUSH;
auto system_cursor(::LPWSTR name = IDC_ARROW) -> ::HCURSOR;
auto system_icon(::LPWSTR name = IDI_APPLICATION) -> ::HICON;
auto resource_icon() -> ::HICON;
auto ui_settings() -> winrt::UISettings;
auto is_dark() -> bool;
auto parse_args() -> std::vector<std::u8string>;
auto parse_args(int argc, char* argv[]) -> std::vector<std::u8string>;

struct CoInit final {
    CoInit(::COINIT coInit = ::COINIT::COINIT_APARTMENTTHREADED);
    ~CoInit();

    operator ::HRESULT() const;

private:
    ::HRESULT result;
};

struct GdiPlus final {
    GdiPlus();
    ~GdiPlus();

private:
    Gdiplus::GdiplusStartupInput input;
    ::ULONG_PTR token;
    Gdiplus::Status status;
};

struct Event final {
    auto create(const std::string& name, std::function<void()>&& callback = 0) -> bool;

    wil::unique_event_nothrow event;
    wil::unique_event_watcher_nothrow watcher;
    std::function<void()> m_callback;
};
}; // namespace glow::system
