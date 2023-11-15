#include <Windows.h>

#include "nlohmann/json.hpp"

#include <filesystem>
#include <print>

#include "../filesystem/filesystem.hxx"
#include "../settings/settings.hxx"
#include "../text/text.hxx"

using json = nlohmann::json;

auto main() -> int
{
    settings s;
    json j = s;
    std::println("{}", j.dump());
    settings test = j;
    std::println("{}", test.name);

    return 0;
}
