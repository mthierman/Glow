#include <stdlib.h>

#include <filesystem>
#include <glow/filesystem.hxx>
#include <glow/log.hxx>
#include <glow/std.hxx>

auto main() -> int {
    // std::filesystem::path path { L"" };
    auto path { glow::filesystem::known_folder() };
    glow::log::log(std::format("{}", path));

    return EXIT_SUCCESS;
}
