#include <Windows.h>

#include <stdlib.h>

#include <format>
#include <print>

#include <glow/glow.hxx>



auto main() -> int {
    std::u8string u8 { u8"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ" };
    std::u16string u16 { u"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ" };

    return EXIT_SUCCESS;
}
