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

#include <functional>
#include <string>

#include <wil/resource.h>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.UI.ViewManagement.h>

namespace winrt {
using namespace winrt::Windows::UI::ViewManagement;
}; // namespace winrt

namespace glow::system {
auto co_initialize(::DWORD coInit = COINIT_APARTMENTTHREADED
                       | COINIT_DISABLE_OLE1DDE) -> wil::unique_couninitialize_call;
auto gdi_plus_startup() -> ::ULONG_PTR;
auto gdi_plus_shutdown(::ULONG_PTR token) -> void;
auto create_process(const std::filesystem::path& path) -> int;
auto get_instance() -> ::HMODULE;
auto abort(::UINT exitCode = EXIT_FAILURE) -> ::UINT;
auto quit(int exitCode = EXIT_SUCCESS) -> int;
auto load_system_brush(int name = NULL_BRUSH) -> ::HBRUSH;
auto load_system_cursor(LPSTR name = IDC_ARROW) -> ::HCURSOR;
auto load_system_icon(LPSTR name = IDI_APPLICATION) -> ::HICON;
auto load_resource_icon() -> ::HICON;
auto make_guid() -> ::GUID;
auto get_ui_settings() -> winrt::UISettings;

struct Event {
    auto create(const std::string& name, std::function<void()>&& callback) -> bool;

    wil::unique_event_nothrow event;
    wil::unique_event_watcher_nothrow watcher;
    std::function<void()> m_callback;
};
}; // namespace glow::system
