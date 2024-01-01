// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <log/log.hxx>

namespace glow::log
{

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

} // namespace glow::log
