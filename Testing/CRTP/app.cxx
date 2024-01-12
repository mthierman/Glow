#include "app.hxx"

auto App::run() -> int
{
    auto app{std::make_unique<App>()};

    // for (auto i = 0; i < 2; i++)
    // {
    //     app->m_vec.emplace_back(std::make_unique<MainWindow>(app->hwnd()))->reveal();
    // }

    auto win = std::make_unique<MainWindow>(app->hwnd());
    win->reveal();

    return glow::gui::message_loop();
}

auto App::wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_NOTIFY: return on_notify(wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto App::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    auto nmhdr{*std::bit_cast<LPNMHDR>(lParam)};

    OutputDebugStringA(std::to_string(nmhdr.idFrom).c_str());
    OutputDebugStringA("\n");

    switch (nmhdr.code)
    {
    case msg::window_create: m_set.insert(nmhdr.idFrom); break;
    case msg::window_close: m_set.erase(nmhdr.idFrom); break;
    }

    if (m_set.empty()) { return close(); }

    else return 0;
}
