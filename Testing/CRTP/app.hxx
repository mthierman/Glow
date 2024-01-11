#pragma once

#include <set>

#include <glow/glow.hxx>
#include "window.hxx"
#include "definitions.hxx"

struct App : public glow::gui::MessageWindow<App>
{
    static auto run() -> int;

    auto handle_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_create(WPARAM wParam, LPARAM lParam) -> int;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;

    std::set<int64_t> m_set;
    std::vector<std::unique_ptr<MainWindow>> m_vec;
};
