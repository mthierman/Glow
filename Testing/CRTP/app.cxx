#include "app.hxx"

auto App::run() -> int
{
    App app;

    // for (auto i = 0; i < 2; i++)
    // {
    //     app.m_vec.emplace_back(std::make_unique<MainWindow>(app.hwnd()))->reveal();
    // }

    auto win = std::make_unique<MainWindow>(app.hwnd());
    win->reveal();

    auto wv = std::make_unique<Browser>(win->hwnd());
    wv->reveal();

    // auto wv = std::make_unique<glow::gui::WebView>(app.hwnd(), "https://www.google.com/");
    // wv->reveal();

    // auto wv = std::make_unique<Browser>();
    // wv->reveal();

    return glow::gui::message_loop();
}

auto App::handle_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_NOTIFY: return on_notify(wParam, lParam);
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }

    return DefWindowProcA(hwnd(), uMsg, wParam, lParam);
}

auto App::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    auto nmhdr{*std::bit_cast<LPNMHDR>(lParam)};

    switch (nmhdr.code)
    {
    case msg::window_create: m_set.insert(nmhdr.idFrom); break;
    case msg::window_close: m_set.erase(nmhdr.idFrom); break;
    }

    if (m_set.empty()) { return close(); }

    else return 0;
}
