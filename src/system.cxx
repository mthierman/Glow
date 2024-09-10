// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/system.hxx>

#include <objbase.h>

#include <stdexcept>

namespace glow::system {
auto co_initialize(::DWORD coInit) -> wil::unique_couninitialize_call {
    return wil::CoInitializeEx(coInit);
}

auto gdi_plus_startup() -> ::ULONG_PTR {
    ::ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input;

    if (auto status { Gdiplus::GdiplusStartup(&token, &input, nullptr) };
        status != Gdiplus::Status::Ok) {
        throw std::runtime_error("GDI+ startup failure");
    }

    return token;
}

auto gdi_plus_shutdown(::ULONG_PTR token) -> void { Gdiplus::GdiplusShutdown(token); }

auto create_process(std::filesystem::path path) -> int {
    ::STARTUPINFOA si { .cb { sizeof(::STARTUPINFOA) },
                        .lpReserved { nullptr },
                        .lpDesktop { nullptr },
                        .lpTitle { nullptr },
                        .dwX { 0 },
                        .dwY { 0 },
                        .dwXSize { 0 },
                        .dwYSize { 0 },
                        .dwXCountChars { 0 },
                        .dwYCountChars { 0 },
                        .dwFillAttribute { 0 },
                        .dwFlags { 0 },
                        .wShowWindow { 0 },
                        .cbReserved2 { 0 },
                        .lpReserved2 { 0 },
                        .hStdInput { nullptr },
                        .hStdOutput { nullptr },
                        .hStdError { nullptr } };

    ::PROCESS_INFORMATION pi {
        .hProcess { nullptr }, .hThread { nullptr }, .dwProcessId { 0 }, .dwThreadId { 0 }
    };

    auto process { path.string() };
    auto pProcess { process.data() };

    ::CreateProcessA(pProcess, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
    ::WaitForSingleObject(pi.hProcess, INFINITE);
    ::CloseHandle(pi.hProcess);
    ::CloseHandle(pi.hThread);

    return 0;
}

auto get_instance() -> ::HMODULE {
    ::HMODULE module;
    ::GetModuleHandleExW(0, nullptr, &module);
    return module;
}

auto abort(::UINT exitCode) -> void { ::ExitProcess(exitCode); }

auto quit(int exitCode) -> void { ::PostQuitMessage(exitCode); }

auto load_system_brush(int name) -> ::HBRUSH {
    return static_cast<::HBRUSH>(::GetStockObject(name));
}

auto load_system_cursor(LPSTR name) -> ::HCURSOR {
    return static_cast<::HCURSOR>(
        ::LoadImageA(nullptr, name, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto load_system_icon(LPSTR name) -> ::HICON {
    return static_cast<::HICON>(
        ::LoadImageA(nullptr, name, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto load_resource_icon() -> ::HICON {
    auto instance { get_instance() };
    return static_cast<::HICON>(
        ::LoadImageW(instance, MAKEINTRESOURCEW(1), IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto make_guid() -> ::GUID {
    ::GUID guid;
    THROW_IF_FAILED(::CoCreateGuid(&guid));

    return guid;
}

auto get_ui_settings() -> winrt::UISettings { return winrt::UISettings(); }

auto Event::create(const std::string& eventName, std::function<void()> callback) -> bool {
    m_callback = std::move(callback);
    watcher.create(event.get(), [this]() { m_callback(); });

    bool exists;
    event.try_create(
        wil::EventOptions::None, glow::text::utf8_to_utf16(eventName).c_str(), nullptr, &exists);

    if (exists) {
        event.SetEvent();
    }

    return exists;
}
}; // namespace glow::system
