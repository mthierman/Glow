// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/system.hxx>

#include <shellapi.h>

#include <iostream>

#include <wil/win32_helpers.h>

#include <glow/color.hxx>
#include <glow/log.hxx>
#include <glow/text.hxx>

namespace glow::system {
auto co_initialize(::COINIT coInit) -> wil::unique_couninitialize_call {
    return wil::CoInitializeEx(coInit | ::COINIT::COINIT_DISABLE_OLE1DDE);
}

auto attach_console() -> void {
    ::AllocConsole();

    wil::unique_file file;

    ::freopen_s(file.addressof(), "CONIN$", "r", stdin);
    ::freopen_s(file.addressof(), "CONOUT$", "w", stdout);
    ::freopen_s(file.addressof(), "CONIN$", "w", stderr);

    std::cin.clear();
    std::cout.clear();
    std::cerr.clear();
    std::clog.clear();
}

auto detach_console() -> void { ::FreeConsole(); }

auto create_process(const std::filesystem::path& path, std::u8string_view commandLine) -> void {
    ::STARTUPINFOW si {};
    si.cb = sizeof(::STARTUPINFOW);

    ::PROCESS_INFORMATION pi {};
    wil::unique_handle hProcess;
    wil::unique_handle hThread;

    pi.hProcess = hProcess.get();
    pi.hThread = hThread.get();

    if (auto converted { glow::text::u16string(commandLine) }; converted) {
        ::CreateProcessW(path.c_str(),
                         glow::text::c_str(converted.value()),
                         nullptr,
                         nullptr,
                         FALSE,
                         0,
                         nullptr,
                         nullptr,
                         &si,
                         &pi);
        ::WaitForSingleObject(pi.hProcess, INFINITE);
    }
}

auto instance() -> ::HMODULE {
    ::HMODULE module;
    ::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT
                             | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                         (LPCWSTR)&instance,
                         &module);

    return module;
}

auto exit_process(unsigned int exitCode) -> void { ::ExitProcess(exitCode); }

auto quit(int exitCode) -> void { ::PostQuitMessage(exitCode); }

auto system_brush(int name) -> ::HBRUSH { return static_cast<::HBRUSH>(::GetStockObject(name)); }

auto system_cursor(::LPWSTR name) -> ::HCURSOR {
    return static_cast<::HCURSOR>(
        ::LoadImageW(nullptr, name, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto system_icon(::LPWSTR name) -> ::HICON {
    return static_cast<::HICON>(
        ::LoadImageW(nullptr, name, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
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

auto argv() -> std::pair<int, std::vector<std::u8string>> {
    int argc { 0 };
    wil::unique_hlocal_ptr<wchar_t*[]> buffer;
    buffer.reset(::CommandLineToArgvW(::GetCommandLineW(), &argc));

    std::vector<std::u8string> argv;

    for (int i = 0; i < argc; i++) {
        if (auto converted { glow::text::u8string(buffer[i]) }; converted) {
            argv.emplace_back(converted.value());
        }
    }

    return { argc, argv };
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

    if (auto converted { glow::text::u8string(buffer.get()) }) {
        return *converted;
    }

    return {};
}

auto get_last_error() -> std::u8string { return format_message(::GetLastError()); }

CoInit::CoInit(::COINIT coInit)
    : result { ::CoInitializeEx(nullptr, coInit | ::COINIT::COINIT_DISABLE_OLE1DDE) } { }

CoInit::~CoInit() {
    if (result == S_OK) {
        ::CoUninitialize();
    }
}

CoInit::operator ::HRESULT() const { return result; }

GdiPlus::GdiPlus()
    : status { Gdiplus::GdiplusStartup(&token, &input, nullptr) } { }

GdiPlus::~GdiPlus() {
    if (status == Gdiplus::Status::Ok) {
        Gdiplus::GdiplusShutdown(token);
    }
}

auto Event::create(std::u8string_view eventName, std::function<void()>&& callback) -> bool {
    m_callback = std::move(callback);
    watcher.create(event.get(), [this]() { m_callback(); });

    bool exists { false };

    if (auto converted { glow::text::u16string(eventName) }; converted) {
        event.try_create(wil::EventOptions::None,
                         reinterpret_cast<const wchar_t*>(converted.value().data()),
                         nullptr,
                         &exists);
    }

    if (exists) {
        event.SetEvent();
    }

    return exists;
}
}; // namespace glow::system
