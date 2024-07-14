#include <print>
#include <stdlib.h>

#include <glow/filesystem.hxx>
#include <glow/messages.hxx>
#include <glow/system.hxx>

namespace fs = std::filesystem;

auto main() -> int {
    auto path { fs::path("D:/GitHub/Glow/test.txt") };

    std::println("{}", fs::exists(path));

    auto watcher { glow::filesystem::Watcher(path) };

    auto read { watcher.readDirectoryChanges() };

    std::println("readDirectoryChanges: {} last_error: {}", read, glow::system::get_last_error());

    return glow::messages::run_loop();

    // return EXIT_SUCCESS;
}
