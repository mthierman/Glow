#include "MessageBox.hxx"

MsgBox::MsgBox(HINSTANCE hInstance) {

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MsgBox::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.hInstance = nullptr;
    wcex.hIcon = storage->application.hIcon;
    wcex.hCursor = storage->application.hCursor;
    wcex.hbrBackground = 0;
    wcex.lpszMenuName = std::wstring(L"MSGBOX").c_str();
    wcex.lpszClassName = std::wstring(L"MSGBOX").c_str();
    wcex.hIconSm = storage->application.hIcon;

    auto atom{RegisterClassExW(&wcex)};

    if (atom == 0)
        util::error("Window failed to register");

    auto hwnd{CreateWindowExW(0, appName.c_str(), appName.c_str(), WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                              nullptr, hInstance, this)};

    if (!hwnd)
        util::error("Window failed to initialize");

    return hwnd;
}

MsgBox::~MsgBox() {}

HWND App::get_hwnd() { return appHwnd; }

__int64 __stdcall App::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    App* app = InstanceFromWndProc<App>(hwnd, msg, lparam);

    if (app)
    {
        switch (msg)
        {
        case WM_CLOSE:
            return app->wm_close(hwnd, msg, wparam, lparam);
        case WM_DESTROY:
            return app->wm_destroy(hwnd, msg, wparam, lparam);
        case WM_ERASEBKGND:
            return app->wm_erasebkgnd(hwnd, msg, wparam, lparam);
        case WM_GETMINMAXINFO:
            return app->wm_getminmaxinfo(hwnd, msg, wparam, lparam);
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

int App::wm_close(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    DestroyWindow(hwnd);

    return 0;
}

int App::wm_destroy(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);

    return 0;
}

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

