#include "browser.hxx"

auto Browser::handle_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CLOSE: return close();
    }

    return DefWindowProcA(hwnd(), uMsg, wParam, lParam);
}
