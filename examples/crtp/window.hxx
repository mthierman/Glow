#pragma once

#include <glow/glow.hxx>
#include "definitions.hxx"
#include "browser.hxx"

struct WindowDimensions
{
    WindowDimensions();

    RECT rect;
    float scale;
    HWND hwnd;
};

struct MainWindow : public glow::window::BaseWindow<MainWindow>
{
    using glow::window::BaseWindow<MainWindow>::BaseWindow;

    MainWindow(HWND app);

    auto default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_close(WPARAM wParam, LPARAM lParam) -> int;
    auto on_size(WPARAM wParam, LPARAM lParam) -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    HWND m_app;
    std::unique_ptr<Browser> m_browser;
};
