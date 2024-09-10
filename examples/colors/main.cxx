#include <print>
#include <stdlib.h>

#include <glow/color.hxx>
#include <glow/log.hxx>
#include <glow/system.hxx>

auto main() -> int {
    auto uiSettings { glow::system::get_ui_settings() };
    auto color { glow::color::get_color(uiSettings, winrt::UIColorType::Accent) };

    glow::log::log("{}", color);

    return EXIT_SUCCESS;
}
