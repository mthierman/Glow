#include <Windows.h>

#include <stdlib.h>

#include <optional>
#include <print>

#include <glow/glow.hxx>

auto wmain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t* argv[]) -> int {
    std::optional<int> opt;

    opt = 32;

    if (opt)
        std::println("{}", *opt);
    else
        std::println("FAIL!");

    return EXIT_SUCCESS;
}
