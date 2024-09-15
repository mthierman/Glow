#include <Windows.h>

#include <stdlib.h>

#include <format>
#include <print>

#include <glow/glow.hxx>

auto main() -> int {
    std::string string { "ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ" };
    std::wstring wstring { L"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ" };
    std::u8string u8 { u8"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ" };
    std::u16string u16 { u"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ" };

    std::println("{}", string);
    std::println("{}", glow::text::to_string(wstring));
    std::println("{}", u8);
    std::println("{}", u16);

    auto path { glow::filesystem::known_folder() };
    std::println("{}", path);
    std::println("{}", glow::text::to_string(path.u8string()));

    return EXIT_SUCCESS;
}
