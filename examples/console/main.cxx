#include <stdlib.h>

#include <print>
#include <string>

#include <glow/filesystem.hxx>
#include <glow/text.hxx>
#include <glow/std.hxx>

auto main() -> int {
    auto knownFolder { glow::filesystem::known_folder() };

    std::println("{}", knownFolder);

    return EXIT_SUCCESS;
}
