// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <Unknwn.h>
#include <dwmapi.h>
#include <gdiplus.h>
#include <objbase.h>
#include <shellapi.h>
#include <comdef.h>
#include <ShlObj.h>
#include <wrl.h>

#include <wil/com.h>
#include <wil/result.h>
#include <wil/resource.h>
#include <wil/win32_helpers.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <algorithm>
#include <bit>
#include <cstdint>
#include <filesystem>
#include <format>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <print>
#include <random>
#include <ranges>
#include <source_location>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <sqlite3.h>

namespace glow
{
// struct Console
// {
//     Console()
//     {
//         m_file = nullptr;

//         AllocConsole();

//         EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE,
//                        MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

//         freopen_s(std::out_ptr(m_file), "CONIN$", "r", stdin);
//         freopen_s(std::out_ptr(m_file), "CONOUT$", "w", stdout);
//         freopen_s(std::out_ptr(m_file), "CONOUT$", "w", stderr);

//         std::cin.clear();
//         std::cout.clear();
//         std::cerr.clear();
//         std::clog.clear();
//     }

//     ~Console()
//     {
//         fclose(m_file);
//         FreeConsole();
//     }

//     FILE* m_file;
// };

// struct Database
// {
//     Database() { m_path = (path_portable() / "db.sqlite"); }

//     auto open()
//     {
//         if (sqlite3_open(m_path.string().c_str(), std::out_ptr(m_db)) != SQLITE_OK)
//             throw std::runtime_error("Failed to open SQLite");
//     }

//     auto write()
//     {
//         std::string sql{"CREATE TABLE CONFIG("
//                         "X INT NOT NULL,"
//                         "Y INT NOT NULL,"
//                         "WIDTH INT NOT NULL,"
//                         "HEIGHT INT NOT NULL,"
//                         "MENU INT NOT NULL,"
//                         "SPLIT INT NOT NULL,"
//                         "MAXIMIZED INT NOT NULL,"
//                         "FULLSCREEN INT NOT NULL,"
//                         "TOPMOST INT NOT NULL,"
//                         "MAIN TEXT NOT NULL,"
//                         "SIDE TEXT NOT NULL);"};

//         if (std::filesystem::exists(m_path))
//         {
//             std::string error;

//             auto dbExec{sqlite3_exec(m_db.get(), sql.c_str(), nullptr, 0, std::out_ptr(error))};

//             if (dbExec != SQLITE_OK)
//             {
//                 std::println("{}", error);
//                 sqlite3_free(error.data());
//             }
//         }
//     }

//   private:
//     struct sqlite3_deleter
//     {
//         auto operator()(sqlite3* db) noexcept -> void { sqlite3_close(db); }
//     };
//     using sqlite3_ptr = std::unique_ptr<sqlite3, sqlite3_deleter>;
//     sqlite3_ptr m_db;
//     std::filesystem::path m_path;
// };

// struct GdiPlus
// {
//     GdiPlus()
//         : m_gdiplusStatus{Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput,
//         nullptr)}
//     {
//         if (m_gdiplusStatus != Gdiplus::Status::Ok)
//             throw std::runtime_error("GDI+ startup failure");
//     }

//     ~GdiPlus()
//     {
//         if (m_gdiplusStatus == Gdiplus::Status::Ok) Gdiplus::GdiplusShutdown(m_gdiplusToken);
//     }

//     constexpr auto operator+(Gdiplus::Status gdiplusStatus) noexcept
//     {
//         return static_cast<std::underlying_type_t<Gdiplus::Status>>(gdiplusStatus);
//     }

//     Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
//     ULONG_PTR m_gdiplusToken;
//     Gdiplus::Status m_gdiplusStatus;
// };

// struct CoInitialize
// {
//     CoInitialize() : m_result{CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)}
//     {
//         if (FAILED(m_result)) throw std::runtime_error("CoInitialize failure");
//     }

//     ~CoInitialize()
//     {
//         if (SUCCEEDED(m_result)) CoUninitialize();
//     }

//     operator HRESULT() const { return m_result; }
//     HRESULT m_result;
// };

// struct WebView2
// {
//     WebView2()
//     {
//         evironmentOptions6 = nullptr;
//         controller = nullptr;
//         controller4 = nullptr;
//         core = nullptr;
//         core20 = nullptr;
//         settings = nullptr;
//         settings8 = nullptr;
//     }

//     wil::com_ptr<ICoreWebView2EnvironmentOptions6> evironmentOptions6;
//     wil::com_ptr<ICoreWebView2Controller> controller;
//     wil::com_ptr<ICoreWebView2Controller4> controller4;
//     wil::com_ptr<ICoreWebView2> core;
//     wil::com_ptr<ICoreWebView2_20> core20;
//     wil::com_ptr<ICoreWebView2Settings> settings;
//     wil::com_ptr<ICoreWebView2Settings8> settings8;
// };

// struct WindowPosition
// {
//     int x{};
//     int y{};
//     int width{};
//     int height{};
//     bool maximized{};
//     bool fullscreen{};
//     bool topmost{};
//     int64_t dpi{};
//     float scale{};
// };

// void to_json(nlohmann::json& j, const WindowPosition& windowPosition)
// {
//     j = nlohmann::json{
//         {"x", windowPosition.x},
//         {"y", windowPosition.y},
//         {"width", windowPosition.width},
//         {"height", windowPosition.height},
//         {"maximized", windowPosition.maximized},
//         {"fullscreen", windowPosition.fullscreen},
//         {"topmost", windowPosition.topmost},
//         {"dpi", windowPosition.dpi},
//         {"scale", windowPosition.scale},
//     };
// }

// void from_json(const nlohmann::json& j, WindowPosition& windowPosition)
// {
//     j.at("x").get_to(windowPosition.x);
//     j.at("y").get_to(windowPosition.y);
//     j.at("width").get_to(windowPosition.width);
//     j.at("height").get_to(windowPosition.height);
//     j.at("maximized").get_to(windowPosition.maximized);
//     j.at("fullscreen").get_to(windowPosition.fullscreen);
//     j.at("topmost").get_to(windowPosition.topmost);
//     j.at("dpi").get_to(windowPosition.dpi);
//     j.at("scale").get_to(windowPosition.scale);
// }

// struct SystemColors
// {
//     SystemColors()
//     {
//         accent = color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
//         accentDark1 =
//         color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1);
//         accentDark2 =
//         color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2);
//         accentDark3 =
//         color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3);
//         accentLight1 =
//             color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1);
//         accentLight2 =
//             color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2);
//         accentLight3 =
//             color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3);
//         background =
//         color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::Background); foreground
//         = color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
//     }

//     std::string accent;
//     std::string accentDark1;
//     std::string accentDark2;
//     std::string accentDark3;
//     std::string accentLight1;
//     std::string accentLight2;
//     std::string accentLight3;
//     std::string background;
//     std::string foreground;
// };

// void to_json(nlohmann::json& j, const SystemColors& systemColors)
// {
//     j = nlohmann::json{
//         {"accent", systemColors.accent},
//         {"accentDark1", systemColors.accentDark1},
//         {"accentDark2", systemColors.accentDark2},
//         {"accentDark3", systemColors.accentDark3},
//         {"accentLight1", systemColors.accentLight1},
//         {"accentLight2", systemColors.accentLight2},
//         {"accentLight3", systemColors.accentLight3},
//         {"background", systemColors.background},
//         {"foreground", systemColors.foreground},
//     };
// }

// void from_json(const nlohmann::json& j, SystemColors& systemColors)
// {
//     j.at("accent").get_to(systemColors.accent);
//     j.at("accentDark1").get_to(systemColors.accentDark1);
//     j.at("accentDark2").get_to(systemColors.accentDark2);
//     j.at("accentDark3").get_to(systemColors.accentDark3);
//     j.at("accentLight1").get_to(systemColors.accentLight1);
//     j.at("accentLight2").get_to(systemColors.accentLight2);
//     j.at("accentLight3").get_to(systemColors.accentLight3);
//     j.at("background").get_to(systemColors.background);
//     j.at("foreground").get_to(systemColors.foreground);
// }

auto argv()
{
    int argc{};
    wil::unique_hlocal_ptr<PWSTR[]> args;
    args.reset(CommandLineToArgvW(GetCommandLineW(), &argc));

    std::vector<std::string> argv;

    for (int i = 0; i < argc; i++)
    {
        auto arg{narrow(args[i])};
        argv.push_back(arg);
    }

    return argv;
}

auto hresult_string(HRESULT errorCode)
{
    auto comError = _com_error(errorCode);
    return std::string(comError.ErrorMessage());
}

auto hresult_check(HRESULT errorCode)
{
    auto errorMessage{hresult_string(errorCode)};

#if _DEBUG
    OutputDebugStringA(errorMessage.c_str());
#endif

    if (SUCCEEDED(errorCode)) return S_OK;

    else throw std::runtime_error(errorMessage);
}

auto hresult_debug(HRESULT errorCode, std::source_location location)
{
    auto errorMessage{hresult_string(errorCode)};
    source_debug(errorMessage, location);
}

auto hresult_print(HRESULT errorCode, std::source_location location)
{
    auto errorMessage{hresult_string(errorCode)};
    source_print(errorMessage, location);
}

auto source_debug(std::string message, std::source_location location)
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

auto source_print(std::string message, std::source_location location)
{
    std::println("Message: {}", message);
    std::println("File: {}", location.file_name());
    std::println("Function: {}", location.function_name());
    std::println("Line: {} | Column: {}", location.line(), location.column());
}

auto message_box(std::string message, UINT type)
{
    auto program{app_name()};

    return MessageBoxA(nullptr, message.c_str(), program.c_str(), type);
}

auto message_box_shell(std::string message, UINT type)
{
    auto program{app_name()};

    return ShellMessageBoxA(GetModuleHandleA(nullptr), nullptr, message.c_str(), program.c_str(),
                            type);
}

auto message_box_stock(std::string message, SHSTOCKICONID icon)
{
    // https://learn.microsoft.com/en-us/windows/win32/api/shellapi/ne-shellapi-shstockiconid
    SHSTOCKICONINFO sii{sizeof(SHSTOCKICONINFO)};

    if (SUCCEEDED(SHGetStockIconInfo(icon, SHGSI_ICONLOCATION, &sii)))
    {
        auto hModule{LoadLibraryExW(sii.szPath, nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

        auto program{app_name()};

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

auto create_process(std::string process)
{
    STARTUPINFOA si{sizeof(STARTUPINFOA)};
    PROCESS_INFORMATION pi{};

    auto server{(path_portable() / process).string()};
    auto pServer{server.data()};

    CreateProcessA(pServer, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

auto known_folder(KNOWNFOLDERID folderId)
{
    // https://learn.microsoft.com/en-us/windows/win32/shell/knownfolderid
    wil::unique_cotaskmem_string buffer;

    if (FAILED(SHGetKnownFolderPath(folderId, 0, nullptr, &buffer)))
        throw std::runtime_error("SHGetKnownFolderPath failure");

    return std::filesystem::path(buffer.get());
}

auto app_name()
{
    std::string buffer;
    _get_pgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return exe.filename().replace_extension("").string();
}

auto path_portable()
{
    std::string buffer;
    _get_pgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return std::filesystem::canonical(exe.remove_filename());
}

auto path_portable_wide()
{
    std::wstring buffer;
    _get_wpgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return std::filesystem::canonical(exe.remove_filename());
}

auto narrow(std::wstring utf16) -> std::string
{
    constexpr int intMax = std::numeric_limits<int>::max();
    if (utf16.length() > static_cast<size_t>(intMax))
        throw std::overflow_error("Input string is too long: size_t doesn't fit into int");

    auto length{WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                    utf16.data(), static_cast<int>(utf16.length()), nullptr, 0,
                                    nullptr, nullptr)};

    std::string utf8(length, 0);

    if (length > 0)
    {
        if (WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS, utf16.data(),
                                static_cast<int>(utf16.length()), utf8.data(), length, nullptr,
                                nullptr) > 0)
            return utf8;

        else throw std::runtime_error("UTF16 to UTF8 conversion failure");
    }

    else throw std::runtime_error("UTF16 to UTF8 conversion failure");
}

auto widen(std::string utf8)
{
    constexpr int intMax = std::numeric_limits<int>::max();
    if (utf8.length() > static_cast<size_t>(intMax))
        throw std::overflow_error("Input string is too long: size_t doesn't fit into int");

    auto length{MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(),
                                    static_cast<int>(utf8.length()), nullptr, 0)};

    std::wstring utf16(length, 0);

    if (length > 0)
    {
        if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(),
                                static_cast<int>(utf8.length()), utf16.data(), length) > 0)
            return utf16;

        else throw std::runtime_error("UTF8 to UTF16 conversion failure");
    }

    else throw std::runtime_error("UTF8 to UTF16 conversion failure");
}

auto randomize(std::string string)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(1.0, 10.0);
    auto randomDouble{dist(mt)};
    auto randomNumber{std::to_string(randomDouble)};
    std::erase(randomNumber, '.');

    return (string + randomNumber);
}

auto random_int64()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int64_t> dist(10000);
    return dist(mt);
}

auto random_int32()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int32_t> dist(10000);
    return dist(mt);
}

auto create_guid()
{
    GUID guid;

    if (SUCCEEDED(CoCreateGuid(&guid))) return guid;

    else throw std::runtime_error("GUID creation failure");
}

auto guid_string(GUID guid)
{
    std::wstring buffer(wil::guid_string_buffer_length, 0);
    auto length{StringFromGUID2(guid, buffer.data(), wil::guid_string_buffer_length)};

    if (length != 0) return narrow(buffer);

    else throw std::runtime_error("String from GUID failure");
}

auto message_loop()
{
    MSG msg{};
    int r{};

    while ((r = GetMessageA(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) { hresult_check(HRESULT_FROM_WIN32(GetLastError())); }

        else
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }

    return 0;
}

auto webview_version()
{
    wil::unique_cotaskmem_string buffer;
    if (FAILED(GetAvailableCoreWebView2BrowserVersionString(nullptr, &buffer)))
        throw std::runtime_error("GetAvailableCoreWebView2BrowserVersionString failure");

    return narrow(buffer.get());
}

auto get_class_info(ATOM& atom, WNDCLASSEXA& wndClass)
{
    if (GetClassInfoExA(GetModuleHandleA(nullptr), MAKEINTATOM(atom), &wndClass)) return true;

    else return false;
}

// auto rect_to_position(const RECT& rect)
// {
//     WindowPosition windowPosition;

//     windowPosition.x = rect.left;
//     windowPosition.y = rect.top;
//     windowPosition.width = rect.right - rect.left;
//     windowPosition.height = rect.bottom - rect.top;

//     return windowPosition;
// }

// auto position_to_rect(const WindowPosition& windowPosition)
// {
//     RECT rect{};

//     rect.left = windowPosition.x;
//     rect.top = windowPosition.y;
//     rect.right = windowPosition.width;
//     rect.bottom = windowPosition.height;

//     return rect;
// }

auto clamp_color(int value) { return std::ranges::clamp(value, 0, 255); }

auto make_colorref(int r, int g, int b)
{
    return RGB(clamp_color(r), clamp_color(g), clamp_color(b));
}

auto color_to_string(winrt::Windows::UI::ViewManagement::UIColorType colorType)
{
    auto settings{winrt::Windows::UI::ViewManagement::UISettings()};
    auto accent{settings.GetColorValue(colorType)};

    return std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", accent.R, accent.G, accent.B, accent.A);
}

auto check_theme()
{
    auto settings{winrt::Windows::UI::ViewManagement::UISettings()};
    auto fg{settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};

    if (((5 * fg.G) + (2 * fg.R) + fg.B) > (8 * 128)) return true;

    return false;
}

auto set_preferred_app_mode()
{
    enum class PreferredAppMode
    {
        Default,
        AllowDark,
        ForceDark,
        ForceLight,
        Max
    };
    using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);

    auto uxtheme{LoadLibraryExA("uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

    if (uxtheme)
    {
        auto ord135{GetProcAddress(uxtheme, MAKEINTRESOURCEA(135))};

        if (ord135)
        {
            auto SetPreferredAppMode{std::bit_cast<fnSetPreferredAppMode>(ord135)};
            SetPreferredAppMode(PreferredAppMode::AllowDark);
        }

        FreeLibrary(uxtheme);
    }
}

auto allow_dark_mode(HWND hWnd, bool enable)
{
    using fnAllowDarkModeForWindow = bool(WINAPI*)(HWND hWnd, bool allow);
    using fnFlushMenuThemes = void(WINAPI*)();
    auto uxtheme{LoadLibraryExA("uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

    if (uxtheme)
    {
        // fnAllowDarkModeForWindow AllowDarkModeForWindow;
        // fnFlushMenuThemes FlushMenuThemes;

        auto ord133{GetProcAddress(uxtheme, MAKEINTRESOURCEA(133))};
        auto ord136{GetProcAddress(uxtheme, MAKEINTRESOURCEA(136))};

        auto AllowDarkModeForWindow{std::bit_cast<fnAllowDarkModeForWindow>(ord133)};
        auto FlushMenuThemes{std::bit_cast<fnFlushMenuThemes>(ord136)};

        AllowDarkModeForWindow(hWnd, enable);
        FlushMenuThemes();
    }

    FreeLibrary(uxtheme);
}

auto icon_application()
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_error()
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_ERROR, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_question()
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_QUESTION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_warning()
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_WARNING, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_information()
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_INFORMATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_winlogo()
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_WINLOGO, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto icon_shield()
{
    return static_cast<HICON>(
        LoadImageA(nullptr, IDI_SHIELD, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}
} // namespace glow
