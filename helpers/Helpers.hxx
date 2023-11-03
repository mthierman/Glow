#pragma once

#include <Windows.h>
#include <ShlObj.h>
#include <dwmapi.h>

#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.viewmanagement.h>

#include <iostream>
#include <string>
#include <random>
#include <filesystem>

namespace glow
{
template <class T, HWND(T::*m_hWnd)> T* InstanceFromWndProc(HWND hWnd, UINT uMsg, LPARAM lParam)
{
    T* pInstance;

    if (uMsg == WM_NCCREATE)
    {
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pInstance = reinterpret_cast<T*>(pCreateStruct->lpCreateParams);
        pInstance->*m_hWnd = hWnd;
        ::SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pInstance));
    }

    else pInstance = reinterpret_cast<T*>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA));

    return pInstance;
}

std::string narrow(std::wstring in)
{
    if (!in.empty())
    {
        auto inSize{static_cast<int>(in.size())};

        auto outSize{::WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                           in.data(), inSize, nullptr, 0, nullptr, nullptr)};

        if (outSize > 0)
        {
            std::string out;
            out.resize(static_cast<size_t>(outSize));

            if (::WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                      in.data(), inSize, out.data(), outSize, nullptr, nullptr) > 0)
                return out;
        }
    }

    return {};
}

std::wstring widen(std::string in)
{
    if (!in.empty())
    {
        auto inSize{static_cast<int>(in.size())};

        auto outSize{
            ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), inSize, nullptr, 0)};

        if (outSize > 0)
        {
            std::wstring out;
            out.resize(static_cast<size_t>(outSize));

            if (::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), inSize, out.data(),
                                      outSize) > 0)
                return out;
        }
    }

    return {};
}

struct Bounds
{
    int x{0};
    int y{0};
    int width{0};
    int height{0};
};

std::wstring randomize(std::wstring in)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(1.0, 10.0);
    auto randomDouble{dist(mt)};
    auto randomNumber{std::to_wstring(randomDouble)};
    std::erase(randomNumber, '.');

    return (in + randomNumber);
}

FILE* create_console()
{
    FILE* f{nullptr};

#ifdef _DEBUG
    ::AllocConsole();
    ::SetConsoleTitleW(L"Console");
    ::EnableMenuItem(::GetSystemMenu(::GetConsoleWindow(), FALSE), SC_CLOSE,
                     MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    // ::GetWindowLongPtrW(::GetConsoleWindow(), GWL_STYLE);
    // ::SetWindowLongPtrW(::GetConsoleWindow(), GWL_STYLE,
    //                     ::GetWindowLongPtrW(::GetConsoleWindow(), GWL_STYLE) & ~WS_CAPTION &
    //                         ~WS_SIZEBOX & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX);
    // ::SetWindowLongPtrW(::GetConsoleWindow(), GWL_EXSTYLE, WS_EX_TOOLWINDOW);
    // ::SetWindowPos(::GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    ::freopen_s(&f, "CONOUT$", "w", stdout);
    ::freopen_s(&f, "CONOUT$", "w", stderr);
    ::freopen_s(&f, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
#endif

    return f;
}

void remove_console(FILE* f)
{
#ifdef _DEBUG
    ::fclose(f);
    ::FreeConsole();
#endif
}

bool check_theme()
{
    auto settings{winrt::Windows::UI::ViewManagement::UISettings()};
    auto fg{settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};

    if (((5 * fg.G) + (2 * fg.R) + fg.B) > (8 * 128)) return true;

    return false;
}

bool set_darkmode(HWND hwnd)
{
    auto dark{TRUE};
    auto light{FALSE};

    if (check_theme())
    {
        ::DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

        return true;
    }

    ::DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &light, sizeof(light));

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

    auto uxtheme{::LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

    if (!uxtheme) return false;

    auto ord135{::GetProcAddress(uxtheme, PCSTR MAKEINTRESOURCEW(135))};

    if (!ord135) return false;

    auto SetPreferredAppMode{reinterpret_cast<fnSetPreferredAppMode>(ord135)};
    SetPreferredAppMode(PreferredAppMode::AllowDark);
    ::FreeLibrary(uxtheme);

    return true;
}

bool window_cloak(HWND hwnd)
{
    auto cloak{TRUE};

    if (FAILED(::DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloak, sizeof(cloak)))) return false;

    return true;
}

bool window_uncloak(HWND hwnd)
{
    auto uncloak{FALSE};

    if (FAILED(::DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &uncloak, sizeof(uncloak)))) return false;

    return true;
}

bool window_mica(HWND hwnd)
{
    MARGINS m{0, 0, 0, ::GetSystemMetrics(SM_CYVIRTUALSCREEN)};
    auto backdrop{DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW};

    if (FAILED(::DwmExtendFrameIntoClientArea(hwnd, &m))) return false;

    if (FAILED(
            ::DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop))))
        return false;

    return true;
}

bool window_maximize(HWND hwnd)
{
    auto style{::GetWindowLongPtrW(hwnd, GWL_STYLE)};

    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    ::GetWindowPlacement(hwnd, &wp);

    if ((style & WS_OVERLAPPEDWINDOW) && wp.showCmd == 3)
    {
        ::ShowWindow(hwnd, SW_SHOWNORMAL);

        return false;
    }

    else
    {
        ::ShowWindow(hwnd, SW_MAXIMIZE);

        return true;
    }
}

bool window_fullscreen(HWND hwnd)
{
    static RECT pos;

    auto style{::GetWindowLongPtrW(hwnd, GWL_STYLE)};

    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = {sizeof(mi)};
        ::GetWindowRect(hwnd, &pos);
        if (::GetMonitorInfoW(::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi))
        {
            ::SetWindowLongPtrW(hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            ::SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                           mi.rcMonitor.right - mi.rcMonitor.left,
                           mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED);
        }

        return true;
    }

    else
    {
        ::SetWindowLongPtrW(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        ::SetWindowPos(hwnd, nullptr, pos.left, pos.top, (pos.right - pos.left),
                       (pos.bottom - pos.top), SWP_FRAMECHANGED);

        return false;
    }
}

bool window_topmost(HWND hwnd)
{
    FLASHWINFO fwi{sizeof(FLASHWINFO)};
    fwi.hwnd = hwnd;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    auto style{::GetWindowLongPtrW(hwnd, GWL_EXSTYLE)};

    if (style & WS_EX_TOPMOST)
    {
        ::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        ::FlashWindowEx(&fwi);

        return false;
    }

    else
    {
        ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        ::FlashWindowEx(&fwi);

        return true;
    }
}

std::filesystem::path known_folder(const KNOWNFOLDERID& id)
{
    wchar_t* buffer{nullptr};

    if (SUCCEEDED(SHGetKnownFolderPath(id, 0, nullptr, &buffer)))
    {
        std::filesystem::path data = std::wstring(buffer);
        CoTaskMemFree(buffer);

        return data;
    }

    else return std::filesystem::path{};
}
} // namespace glow
