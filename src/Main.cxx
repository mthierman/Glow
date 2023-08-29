#include "Glow.hxx"

HWND make_window(HINSTANCE);
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int);
static __int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

HWND make_window(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = L"TEST";
    wcex.lpszMenuName = L"TEST";
    wcex.lpfnWndProc = WndProc;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.hCursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wcex.hIcon =
        (HICON)LoadImageW(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE);
    wcex.hIconSm =
        (HICON)LoadImageW(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE);

    auto atom{RegisterClassExW(&wcex)};

    if (atom == 0)
        glow::error("Window failed to register");

    auto hwnd{CreateWindowExW(0, L"TEST", L"TEST", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                              hInstance, nullptr)};

    if (!hwnd)
        glow::error("Window failed to initialize");

    return hwnd;
}

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    auto hwnd{make_window(hInstance)};

    ShowWindow(hwnd, SW_SHOWDEFAULT);

    MSG msg;
    int r;

    while ((r = GetMessageW(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1)
            return 0;

        else
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return 0;
}

static __int64 __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    return DefWindowProcW(hwnd, msg, wparam, lparam);
}
