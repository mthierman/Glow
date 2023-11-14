#pragma once

#include <Windows.h>

#include "../gui/app.hxx"

namespace glow
{
struct App : glow::gui::App
{
    using glow::gui::App::App;

  private:
    auto WndProc(HWND h, UINT m, WPARAM w, LPARAM l) -> LRESULT override;
    static auto EnumChildProc(HWND h, LPARAM l) -> BOOL;

    auto OnNotify(HWND h) -> int;
    auto OnWindowPosChanged(HWND h) -> int;
};

auto App::WndProc(HWND h, UINT m, WPARAM w, LPARAM l) -> LRESULT
{
    switch (m)
    {
    case WM_NOTIFY: return OnNotify(h);
    case WM_WINDOWPOSCHANGED: return OnWindowPosChanged(h);
    }

    return ::DefWindowProc(h, m, w, l);
}

auto CALLBACK App::EnumChildProc(HWND h, LPARAM l) -> BOOL
{
    auto childId{::GetWindowLongPtr(h, GWL_ID)};

    auto rcParent{(LPRECT)l};

    if (childId == 1)
        ::SetWindowPos(h, nullptr, 0, 0, (rcParent->right - rcParent->left),
                       (rcParent->bottom - rcParent->top), SWP_NOZORDER);

    return 1;
}

auto App::OnNotify(HWND h) -> int
{

    RECT clientRect{0};
    ::GetClientRect(h, &clientRect);
    ::EnumChildWindows(h, EnumChildProc, (LPARAM)&clientRect);

    return 0;
}

auto App::OnWindowPosChanged(HWND h) -> int
{
    RECT clientRect{0};
    ::GetClientRect(h, &clientRect);
    ::EnumChildWindows(h, EnumChildProc, (LPARAM)&clientRect);

    return 0;
}
} // namespace glow
