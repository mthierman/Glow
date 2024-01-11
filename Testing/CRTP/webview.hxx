#pragma once

#include <glow/glow.hxx>
#include "definitions.hxx"

struct WebView : public glow::gui::BaseWindow<WebView>
{
    using glow::gui::BaseWindow<WebView>::BaseWindow;

    WebView(HWND parent, std::string url);

    auto handle_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_close(WPARAM wParam, LPARAM lParam) -> int;

    HWND m_parent;
    std::string m_url;
};
