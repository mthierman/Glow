#pragma once

#include <set>

#include <glow/glow.hxx>

#include "definitions.hxx"
#include "browser.hxx"
#include "window.hxx"

struct App : public glow::gui::MessageWindow<App>
{
    static auto run() -> int;

    using glow::gui::MessageWindow<App>::MessageWindow;

    // auto handle_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    // auto on_notify(WPARAM wParam, LPARAM lParam) -> int;

    std::set<int64_t> m_set;
    std::vector<std::unique_ptr<MainWindow>> m_vec;
};
