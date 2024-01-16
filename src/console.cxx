// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "console.hxx"

namespace glow::console
{
Console::Console()
{
    AllocConsole();

    // Disable close button
    // EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE,
    //                MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

    freopen_s(m_file.addressof(), "CONIN$", "r", stdin);
    freopen_s(m_file.addressof(), "CONOUT$", "w", stdout);
    freopen_s(m_file.addressof(), "CONOUT$", "w", stderr);

    std::cin.clear();
    std::cout.clear();
    std::cerr.clear();
    std::clog.clear();
}

Console::~Console() { FreeConsole(); }

auto argv() -> std::vector<std::string>
{
    int argc{};
    wil::unique_hlocal_ptr<PWSTR[]> buffer;
    buffer.reset(CommandLineToArgvW(GetCommandLineW(), &argc));

    std::vector<std::string> argv;

    for (int i = 0; i < argc; i++)
    {
        argv.push_back(glow::text::narrow(buffer[i]));
    }

    return argv;
}

auto system_error_string(HRESULT errorCode) -> std::string
{
    return std::string(std::system_category().message(errorCode));
}

auto hresult_string(HRESULT errorCode) -> std::string
{
    auto comError = _com_error(errorCode);
    return std::string(comError.ErrorMessage());
}

auto last_error() -> HRESULT { return HRESULT_FROM_WIN32(GetLastError()); }

auto last_error_string() -> std::string { return hresult_string(last_error()); }

auto hresult_check(HRESULT errorCode) -> HRESULT
{
    auto errorMessage{hresult_string(errorCode)};

#if _DEBUG
    OutputDebugStringA(errorMessage.c_str());
#endif

    if (SUCCEEDED(errorCode)) return S_OK;

    else throw std::runtime_error(errorMessage);
}

auto hresult_debug(HRESULT errorCode, std::source_location location) -> void
{
    auto errorMessage{hresult_string(errorCode)};
    source_debug(errorMessage, location);
}

auto hresult_print(HRESULT errorCode, std::source_location location) -> void
{
    auto errorMessage{hresult_string(errorCode)};
    source_print(errorMessage, location);
}

auto source_debug(std::string message, std::source_location location) -> void
{
    OutputDebugStringA("Message: ");
    OutputDebugStringA(message.c_str());
    OutputDebugStringA("\n");
    OutputDebugStringA(("File: " + std::string(location.file_name())).c_str());
    OutputDebugStringA("\n");
    OutputDebugStringA(("Function: " + std::string(location.function_name())).c_str());
    OutputDebugStringA("\n");
    OutputDebugStringA(("Line: " + std::to_string(location.line())).c_str());
    OutputDebugStringA(" | ");
    OutputDebugStringA(("Column: " + std::to_string(location.column())).c_str());
    OutputDebugStringA("\n");
}

auto source_print(std::string message, std::source_location location) -> void
{
    std::println("Message: {}", message);
    std::println("File: {}", location.file_name());
    std::println("Function: {}", location.function_name());
    std::println("Line: {} | Column: {}", location.line(), location.column());
}

auto message_box(std::string message, unsigned int type) -> int
{
    return MessageBoxA(nullptr, message.c_str(), glow::filesystem::app_name().c_str(), type);
}

auto message_box_shell(std::string message, unsigned int type) -> int
{
    return ShellMessageBoxA(GetModuleHandleA(nullptr), nullptr, message.c_str(),
                            glow::filesystem::app_name().c_str(), type);
}

auto message_box_stock(std::string message, SHSTOCKICONID icon) -> void
{
    // https://learn.microsoft.com/en-us/windows/win32/api/shellapi/ne-shellapi-shstockiconid
    SHSTOCKICONINFO sii{sizeof(SHSTOCKICONINFO)};

    if (SUCCEEDED(SHGetStockIconInfo(icon, SHGSI_ICONLOCATION, &sii)))
    {
        auto hModule{LoadLibraryExW(sii.szPath, nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

        auto program{glow::filesystem::app_name()};

        MSGBOXPARAMS params{sizeof(MSGBOXPARAMS)};
        params.hInstance = hModule;
        params.lpszText = message.c_str();
        params.lpszCaption = program.c_str();
        params.dwStyle = MB_USERICON;
        params.lpszIcon = MAKEINTRESOURCE(-sii.iIcon);
        params.lpfnMsgBoxCallback = nullptr;
        MessageBoxIndirectA(&params);

        FreeLibrary(hModule);
    }
}

auto create_process(std::string process) -> int
{
    STARTUPINFOA si{sizeof(STARTUPINFOA)};
    PROCESS_INFORMATION pi{};

    auto server{(glow::filesystem::path_portable() / process).string()};
    auto pServer{server.data()};

    CreateProcessA(pServer, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
} // namespace glow::console
