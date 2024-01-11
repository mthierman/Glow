#include "webview.hxx"

WebView::WebView(HWND parent, std::string url)
    : BaseWindow("WebView", WS_CHILD, 0, 0, 0, 0, 0, parent, std::bit_cast<HMENU>(m_id))
{
    m_parent = parent;
    m_url = url;
}

auto WebView::handle_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CLOSE: return on_close(wParam, lParam);
    }

    return DefWindowProcA(hwnd(), uMsg, wParam, lParam);
}

auto WebView::on_close(WPARAM wParam, LPARAM lParam) -> int { return close(); }

// struct Browser : public glow::gui::WebView<Browser>
// {
//     using glow::gui::WebView<Browser>::WebView;

//     auto handle_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
//     auto on_close(WPARAM wParam, LPARAM lParam) -> int;
// };

// auto Browser::handle_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
// {
//     switch (uMsg)
//     {
//     case WM_CLOSE: return on_close(wParam, lParam);
//     }

//     return DefWindowProcA(hwnd(), uMsg, wParam, lParam);
// }

// auto Browser::on_close(WPARAM wParam, LPARAM lParam) -> int
// {
//     notify(m_app, msg::window_close);

//     return close();
// }
