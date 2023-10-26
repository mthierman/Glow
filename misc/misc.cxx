int App::wm_erasebkgnd(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    InvalidateRect(hwnd, nullptr, true);
    PAINTSTRUCT ps{};
    HDC hdc = BeginPaint(hwnd, &ps);

    if (storage->settings.appTheme == "dark")
        FillRect(hdc, &ps.rcPaint, storage->application.darkBrush);

    else
        FillRect(hdc, &ps.rcPaint, storage->application.lightBrush);

    EndPaint(hwnd, &ps);

    return 1;
}

int App::wm_getminmaxinfo(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO minmax{(LPMINMAXINFO)lparam};
    minmax->ptMinTrackSize.x = 500;
    minmax->ptMinTrackSize.y = 500;

    return 0;
}

case WM_SIZE:
{
    RECT r;
    GetClientRect(hwnd, &r);
    EnumChildWindows(hwnd, EnumChildProcCallback, (LPARAM)&r);

    return 0;
}

// virtual int EnumChildProc(HWND, LPARAM);

int __stdcall Window::EnumChildProcCallback(HWND hwnd, LPARAM lparam)
{
    auto child{GetWindowLongPtrW(hwnd, GWL_ID)};
    auto p{(LPRECT)lparam};

    SetWindowPos(hwnd, nullptr, 0, 0, p->right, p->bottom, SWP_NOZORDER);

    return 1;
}

int __stdcall Window::EnumChildProcCallback(HWND hwnd, LPARAM lparam)
{
    Window* window = InstanceFromWndProc<Window, Window, &Window::m_hwnd>(hwnd, 0, lparam);

    if (window)
    {
        return window->EnumChildProc(hwnd, lparam);
    }

    return 1;
}

int __stdcall Window::EnumChildProc(HWND hwnd, LPARAM lparam)
{
    auto child{GetWindowLongPtrW(hwnd, GWL_ID)};
    auto p{(LPRECT)lparam};

    SetWindowPos(hwnd, nullptr, 0, 0, p->right, p->bottom, SWP_NOZORDER);

    return 1;
}
