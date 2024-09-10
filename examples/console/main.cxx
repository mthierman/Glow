#include <print>
#include <stdlib.h>

#include <glow/filesystem.hxx>
#include <glow/log.hxx>

auto main() -> int {
    auto knownFolder { glow::filesystem::known_folder() };

    glow::log::log(knownFolder);

    auto path { glow::filesystem::known_folder() / L"Test" };

    return EXIT_SUCCESS;
}
