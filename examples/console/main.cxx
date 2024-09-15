#include <stdlib.h>

#include <format>
#include <print>

#include <glow/glow.hxx>

auto main() -> int {
    std::u8string u8 { u8"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ" };

    auto raw = (char*)u8.c_str();

    std::println("{}", u8);

    return EXIT_SUCCESS;
}
