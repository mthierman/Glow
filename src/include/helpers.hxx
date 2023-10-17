#pragma once

#include <Windows.h>
#include <ShlObj.h>
#include <dwmapi.h>
#include <string>
#include <random>
#include <filesystem>
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.UI.ViewManagement.h"

namespace glow::win32
{
struct Bounds
{
    int x{0};
    int y{0};
    int width{0};
    int height{0};
};

std::string randomize(std::string in)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(1.0, 10.0);
    auto randomDouble{dist(mt)};
    auto randomNumber{std::to_string(randomDouble)};
    std::erase(randomNumber, '.');

    return (in + randomNumber);
}

bool is_darkmode()
{
    auto settings{winrt::Windows::UI::ViewManagement::UISettings()};
    auto fg{settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};

    if (((5 * fg.G) + (2 * fg.R) + fg.B) > (8 * 128))
        return true;

    return false;
}

bool set_darkmode(HWND hwnd)
{
    auto dark{TRUE};
    auto light{FALSE};

    if (is_darkmode())
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

        return true;
    }

    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &light, sizeof(light));

    return false;
}

bool set_darktitle()
{
    enum PreferredAppMode
    {
        Default,
        AllowDark,
        ForceDark,
        ForceLight,
        Max
    };

    using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);

    auto uxtheme{LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

    if (!uxtheme)
        return false;

    auto ord135{GetProcAddress(uxtheme, PCSTR MAKEINTRESOURCEW(135))};

    if (!ord135)
        return false;

    auto SetPreferredAppMode{reinterpret_cast<fnSetPreferredAppMode>(ord135)};
    SetPreferredAppMode(PreferredAppMode::AllowDark);
    FreeLibrary(uxtheme);

    return true;
}

bool set_mica(HWND hwnd)
{
    MARGINS m{0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
    auto backdrop{DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW};

    if (FAILED(DwmExtendFrameIntoClientArea(hwnd, &m)))
        return false;

    if (FAILED(
            DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop))))
        return false;

    return true;
}

bool window_cloak(HWND hwnd)
{
    auto cloak{TRUE};

    if (FAILED(DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloak, sizeof(cloak))))
        return false;

    return true;
}

bool window_uncloak(HWND hwnd)
{
    auto uncloak{FALSE};

    if (FAILED(DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &uncloak, sizeof(uncloak))))
        return false;

    return true;
}

std::filesystem::path path_appdata(std::string n)
{
    PWSTR buffer;
    std::filesystem::path data{};

    if (FAILED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer)))
        return std::filesystem::path{};

    data = std::wstring(buffer) + std::filesystem::path::preferred_separator + to_wstring(n);

    CoTaskMemFree(buffer);

    if (!std::filesystem::exists(data))
        std::filesystem::create_directory(data);

    return data;
}
} // namespace glow::win32
