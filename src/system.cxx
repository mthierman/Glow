// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/system.hxx>

#include <stdexcept>

#include <glow/color.hxx>
#include <glow/text.hxx>

namespace glow::system {
auto co_initialize(::DWORD coInit) -> wil::unique_couninitialize_call {
    return wil::CoInitializeEx(coInit);
}

auto create_process(const std::filesystem::path& path) -> int {
    ::STARTUPINFOW si { .cb { sizeof(::STARTUPINFOW) },
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

    auto process { path.wstring() };
    auto pProcess { process.data() };

    ::CreateProcessW(pProcess, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
    ::WaitForSingleObject(pi.hProcess, INFINITE);
    ::CloseHandle(pi.hProcess);
    ::CloseHandle(pi.hThread);

    return 0;
}

auto instance() -> ::HMODULE {
    ::HMODULE module;
    ::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, nullptr, &module);

    return module;
}

auto exit_process(::UINT exitCode) -> void { ::ExitProcess(exitCode); }

auto quit(int exitCode) -> void { ::PostQuitMessage(exitCode); }

auto system_brush(int name) -> ::HBRUSH { return static_cast<::HBRUSH>(::GetStockObject(name)); }

auto system_cursor(LPSTR name) -> ::HCURSOR {
    return static_cast<::HCURSOR>(
        ::LoadImageA(nullptr, name, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto system_icon(LPSTR name) -> ::HICON {
    return static_cast<::HICON>(
        ::LoadImageA(nullptr, name, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto resource_icon() -> ::HICON {
    return static_cast<::HICON>(
        ::LoadImageW(instance(), MAKEINTRESOURCEW(1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE));
}

auto ui_settings() -> winrt::UISettings { return winrt::UISettings(); }

auto is_dark() -> bool {
    auto bg { glow::color::Color(winrt::UIColorType::Background) };

    return (((5 * bg.g) + (2 * bg.r) + bg.b) < (8 * 128)) ? true : false;
}

auto parse_args(int argc, char* argv[]) -> std::vector<std::u8string> {
    std::vector<std::u8string> args;

    for (int i = 0; i < argc; i++) {
        args.emplace_back(glow::text::to_u8string(argv[i]));
    }

    return args;
}

GdiPlus::GdiPlus() {
    if (Gdiplus::GdiplusStartup(&token, &input, nullptr) != Gdiplus::Status::Ok) {
        throw std::runtime_error("GDI+ startup failure");
    }
}

GdiPlus::~GdiPlus() { Gdiplus::GdiplusShutdown(token); }

auto Event::create(const std::string& eventName, std::function<void()>&& callback) -> bool {
    m_callback = std::move(callback);
    watcher.create(event.get(), [this]() { m_callback(); });

    bool exists;
    event.try_create(
        wil::EventOptions::None, glow::text::to_wstring(eventName).c_str(), nullptr, &exists);

    if (exists) {
        event.SetEvent();
    }

    return exists;
}
}; // namespace glow::system
