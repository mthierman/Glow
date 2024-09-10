#include <stdlib.h>

#include <print>

#include <glow/glow.hxx>

auto main() -> int {
    //
    auto ui { glow::system::get_ui_settings() };
    auto color { glow::color::get_color(ui) };

    // auto s { glow::color::to_string(color) };
    // auto w { glow::color::to_wstring(color) };

    // std::println("{}", s);
    // std::println(L"{}", w);

    auto s { std::format("{}", color) };
    auto w { std::format(L"{}", color) };

    // std::println("{}, {}, {}, {}", color.R, color.G, color.B, color.A);
    // std::println("{}", glow::color::to_hex(color));

    // std::println("{}", glow::color::to_string(color));
    // std::println(L"{}", glow::color::to_wstring(color));

    // std::println("{}", color);
    // std::println(L"{}", color);

    return EXIT_SUCCESS;
}
