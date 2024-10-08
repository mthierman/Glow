#include <Windows.h>

#include <stdlib.h>

#include <glow/glow.hxx>

#include <print>

#include <icu.h>

auto wmain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t* argv[]) -> int {
    auto text { std::u8string(u8"test") };
    // std::println("text - length: {} size: {} string: {}", text.length(), text.size(), text);

    // auto view = std::u16string_view(text);
    // std::println("view - length: {} size: {}", view.length(), view.size());

    // glow::text::upper(text);

    if (auto converted { glow::text::upper(text) }; converted.has_value()) {
        std::println(
            "{}, {}, {}", converted.value(), converted.value().length(), converted.value().size());
    }

    // std::println("{}", );

    return EXIT_SUCCESS;
}
