#include <print>
#include <stdlib.h>

#include <glow/color.hxx>
#include <glow/log.hxx>
#include <glow/system.hxx>

auto main() -> int {
    auto color { glow::color::system(glow::system::ui_settings(), winrt::UIColorType::Accent) };

    glow::log::log("{}", color);

    return EXIT_SUCCESS;
}
