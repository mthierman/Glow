#include <stdlib.h>

#include <print>

#include <glow/glow.hxx>

auto main() -> int {
    //
    auto ui { glow::system::get_ui_settings() };
    auto color { glow::color::get_color(ui) };

    std::println("{}, {}, {}, {}", color.R, color.G, color.B, color.A);
    std::println("{}", glow::color::to_hex(color));
    std::println("{}", color);

    return EXIT_SUCCESS;
}
