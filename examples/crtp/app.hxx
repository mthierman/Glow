#pragma once

#include <set>
#include <glow/glow.hxx>
#include "definitions.hxx"
#include "window.hxx"

struct App : public glow::window::MessageWindow<App>
{
    using glow::window::MessageWindow<App>::MessageWindow;

    static auto run() -> int;

    auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;

    std::set<uint64_t> m_set;
    std::vector<std::unique_ptr<MainWindow>> m_vec;
};
