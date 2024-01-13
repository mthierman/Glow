#pragma once

#include <glow/glow.hxx>
#include "definitions.hxx"

struct Browser : public glow::window::WebView<Browser>
{
    using glow::window::WebView<Browser>::WebView;
};
