#pragma once

#include <set>

#include <glow/glow.hxx>

#include "definitions.hxx"
#include "window.hxx"

struct App : public glow::gui::MessageWindow<App>
{
    using glow::gui::MessageWindow<App>::MessageWindow;

    static auto run() -> int;

    auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;

    std::set<int64_t> m_set;
    std::vector<std::unique_ptr<MainWindow>> m_vec;
};
