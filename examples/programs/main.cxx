#include <Windows.h>

#include <stdlib.h>

#include <print>

#include <glow/glow.hxx>

auto wmain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t* argv[]) -> int {
    // glow::log::log(u8"TEST");
    auto u16 = glow::text::u16string(u8"TEST");

    // glow::log::log(u16);
    if (u16)
        ::OutputDebugStringW(reinterpret_cast<wchar_t*>(u16->data()));

    return EXIT_SUCCESS;
}
