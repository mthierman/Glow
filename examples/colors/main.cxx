#include <print>
#include <stdlib.h>

#include <glow/color.hxx>
#include <glow/system.hxx>

auto main() -> int {
    auto uiSettings { glow::system::get_ui_settings() };
    auto color { glow::color::get_color(uiSettings, winrt::UIColorType::Accent) };

    glow::system::dbg("{}", color);

    return EXIT_SUCCESS;
}
