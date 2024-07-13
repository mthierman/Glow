#include <print>
#include <stdlib.h>

#include <glow/filesystem.hxx>
#include <glow/messages.hxx>

namespace fs = std::filesystem;

auto main() -> int {
    auto path { fs::path("D:/GitHub/Glow/test.txt") };

    std::println("{}", fs::exists(path));

    auto watcher { glow::filesystem::Watcher(path) };

    return glow::messages::run_loop();

    // return EXIT_SUCCESS;
}
