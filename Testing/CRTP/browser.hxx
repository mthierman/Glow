#pragma once

#include <glow/glow.hxx>
#include "definitions.hxx"

struct Browser : public glow::gui::WebView<Browser>
{
    using glow::gui::WebView<Browser>::WebView;

    // auto handle_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
};
