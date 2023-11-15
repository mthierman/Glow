#include <Windows.h>

#include "nlohmann/json.hpp"

#include <print>

#include "../filesystem/filesystem.hxx"
#include "../settings/settings.hxx"
#include "../text/text.hxx"

auto main() -> int
{
    std::println("{}", glow::filesystem::known_folder(FOLDERID_LocalAppData).string());
    std::println("{}", glow::filesystem::path_portable().string());
    std::println("{}", glow::text::randomize("Randomized!"));

    nlohmann::json j{};

    return 0;
}
