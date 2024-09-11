#include <stdlib.h>

#include <print>

#include <glow/glow.hxx>

auto main() -> int {
    auto accent { glow::color::system(winrt::UIColorType::Accent) };

    std::println("{}", accent);

    return EXIT_SUCCESS;
}
