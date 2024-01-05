// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/console.hxx>

namespace glow::console
{

Console::Console()
{
    AllocConsole();

    EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE,
                   MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

    freopen_s(std::out_ptr(pFile), "CONIN$", "r", stdin);
    freopen_s(std::out_ptr(pFile), "CONOUT$", "w", stdout);
    freopen_s(std::out_ptr(pFile), "CONOUT$", "w", stderr);

    std::cin.clear();
    std::cout.clear();
    std::cerr.clear();
    std::clog.clear();
}

Console::~Console()
{
    fclose(pFile);
    FreeConsole();
}

auto argv() -> std::vector<std::string>
{
    int argc{};
    wil::unique_hlocal_ptr<PWSTR[]> args;
    args.reset(CommandLineToArgvW(GetCommandLineW(), &argc));

    std::vector<std::string> argv;

    for (int i = 0; i < argc; i++)
    {
        auto arg{glow::text::narrow(args[i])};
        argv.push_back(arg);
    }

    return argv;
}

auto hresult_to_string(HRESULT errorCode) -> std::string
{
    auto comError = _com_error(errorCode);
    return std::string(comError.ErrorMessage());
}

auto hresult_check(HRESULT errorCode) -> HRESULT
{
    if (SUCCEEDED(errorCode)) return S_OK;
    else throw std::runtime_error(glow::console::hresult_to_string(errorCode));
}

auto debug_hr(HRESULT errorCode, std::source_location location) -> void
{
    auto message{hresult_to_string(errorCode)};
    debug(message, location);
}

auto print_hr(HRESULT errorCode, std::source_location location) -> void
{
    auto message{hresult_to_string(errorCode)};
    print(message, location);
}
auto debug(std::string message, std::source_location location) -> void
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

auto print(std::string message, std::source_location location) -> void
{
    std::println("Message: {}", message);
    std::println("File: {}", location.file_name());
    std::println("Function: {}", location.function_name());
    std::println("Line: {} | Column: {}", location.line(), location.column());
}

auto box(std::string message, UINT type) -> int
{
    auto program{glow::filesystem::program_name()};

    return MessageBoxA(nullptr, message.c_str(), program.c_str(), type);
}

auto shell(std::string message, UINT type) -> int
{
    auto program{glow::filesystem::program_name()};

    return ShellMessageBoxA(GetModuleHandleA(nullptr), nullptr, message.c_str(), program.c_str(),
                            type);
}

auto stock(std::string message, SHSTOCKICONID icon) -> void
{
    // https://learn.microsoft.com/en-us/windows/win32/api/shellapi/ne-shellapi-shstockiconid
    SHSTOCKICONINFO sii{sizeof(SHSTOCKICONINFO)};

    if (SUCCEEDED(SHGetStockIconInfo(icon, SHGSI_ICONLOCATION, &sii)))
    {
        auto hModule{LoadLibraryExW(sii.szPath, nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

        auto program{glow::filesystem::program_name()};

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

} // namespace glow::console
