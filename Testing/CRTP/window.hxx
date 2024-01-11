#pragma once

#include <glow/glow.hxx>
#include "definitions.hxx"

struct MainWindow : public glow::gui::BaseWindow<MainWindow>
{
    using glow::gui::BaseWindow<MainWindow>::BaseWindow;

    MainWindow(HWND app);

    auto handle_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_create(WPARAM wParam, LPARAM lParam) -> int;
    auto on_close(WPARAM wParam, LPARAM lParam) -> int;

    HWND m_app;
};
