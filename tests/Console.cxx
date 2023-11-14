#include <Windows.h>

#include <print>

#include "../console/console.hxx"
#include "../filesystem/filesystem.hxx"
#include "../text/text.hxx"

auto main() -> int
{
    std::println("{}", glow::filesystem::known_folder(FOLDERID_LocalAppData).string());
    std::println("{}", glow::filesystem::path_portable().string());
    std::println("{}", glow::text::randomize("Randomized!"));

    return 0;
}
