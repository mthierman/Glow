#include "window.hxx"

WebView::WebView(HWND app) : BaseWindow()
{
    m_app = app;
    notify(m_app, msg::window_create);
}

auto WebView::handle_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CLOSE: return on_close(wParam, lParam);
    }

    return DefWindowProcA(hwnd(), uMsg, wParam, lParam);
}

auto WebView::on_close(WPARAM wParam, LPARAM lParam) -> int
{
    notify(m_app, msg::window_close);

    return close();
}
