// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "gui.hxx"

namespace glow
{
auto set_preferred_app_mode() -> void
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

    auto uxtheme{::LoadLibraryExA("uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

    if (uxtheme)
    {
        auto ord135{::GetProcAddress(uxtheme, MAKEINTRESOURCEA(135))};

        if (ord135)
        {
            auto SetPreferredAppMode{reinterpret_cast<fnSetPreferredAppMode>(ord135)};
            SetPreferredAppMode(PreferredAppMode::AllowDark);
        }

        FreeLibrary(uxtheme);
    }
}

auto allow_dark_mode(::HWND hWnd, bool enable) -> void
{
    using fnAllowDarkModeForWindow = bool(WINAPI*)(::HWND hWnd, bool allow);
    using fnFlushMenuThemes = void(WINAPI*)();
    auto uxtheme{::LoadLibraryExA("uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

    if (uxtheme)
    {
        // fnAllowDarkModeForWindow AllowDarkModeForWindow;
        // fnFlushMenuThemes FlushMenuThemes;

        auto ord133{::GetProcAddress(uxtheme, MAKEINTRESOURCEA(133))};
        auto ord136{::GetProcAddress(uxtheme, MAKEINTRESOURCEA(136))};

        auto AllowDarkModeForWindow{reinterpret_cast<fnAllowDarkModeForWindow>(ord133)};
        auto FlushMenuThemes{reinterpret_cast<fnFlushMenuThemes>(ord136)};

        AllowDarkModeForWindow(hWnd, enable);
        FlushMenuThemes();
    }

    ::FreeLibrary(uxtheme);
}
} // namespace glow
