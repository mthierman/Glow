#include <Windows.h>

#include <stdlib.h>

#include <print>

#include <glow/glow.hxx>

auto wmain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t* argv[]) -> int {
    glow::log::log(u8"TEST");

    return EXIT_SUCCESS;
}
