// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <logging/logging.hxx>

namespace glow::logging
{

auto hr(HRESULT hresult) -> void
{
    std::string message{std::system_category().message(hresult)};
    std::println("{}", message);
}

auto com_error(HRESULT hResult) -> void
{
    _com_error comError(hResult);
    OutputDebugStringA(comError.ErrorMessage());
}

auto debug(std::string string, std::source_location location) -> void
{
    OutputDebugStringA(("file_name: " + std::string(location.file_name()) + "\n").c_str());
    OutputDebugStringA(("function_name: " + std::string(location.function_name()) + "\n").c_str());
    OutputDebugStringA(("line: " + std::to_string(location.line()) + "\n").c_str());
    OutputDebugStringA(("column: " + std::to_string(location.column()) + "\n").c_str());
    OutputDebugStringA((string + "\n").c_str());
}

auto box_msg(std::string message, UINT type) -> void
{
    MessageBoxA(nullptr, message.c_str(), "Message", type);
}

auto box_err(std::string message) -> void
{
    std::string errorMessage = message + ". Error: " + std::to_string(GetLastError());
    MessageBoxA(nullptr, errorMessage.c_str(), "Error", MB_ICONERROR);
}

auto box_icon(std::string message, SHSTOCKICONID icon) -> void
{
    // https://learn.microsoft.com/en-us/windows/win32/api/shellapi/ne-shellapi-shstockiconid
    SHSTOCKICONINFO sii{sizeof(SHSTOCKICONINFO)};

    if (SUCCEEDED(SHGetStockIconInfo(icon, SHGSI_ICONLOCATION, &sii)))
    {
        HMODULE hInstance{LoadLibraryW(sii.szPath)};

        MSGBOXPARAMS params{sizeof(MSGBOXPARAMS)};
        params.hInstance = hInstance;
        params.lpszText = message.c_str();
        params.lpszCaption = "Message";
        params.dwStyle = MB_USERICON;
        params.lpszIcon = MAKEINTRESOURCE(-sii.iIcon);
        params.lpfnMsgBoxCallback = nullptr;
        MessageBoxIndirectA(&params);

        FreeLibrary(hInstance);
    }
}

auto shell_msg(std::string message) -> void
{
    ShellMessageBoxA(GetModuleHandleA(nullptr), nullptr, message.c_str(), "Message", 0);
}

auto shell_err(std::string message) -> void
{
    std::string errorMessage = message + ". Error: " + std::to_string(GetLastError());
    ShellMessageBoxA(GetModuleHandleA(nullptr), nullptr, errorMessage.c_str(), "Error", 0);
}

} // namespace glow::logging
