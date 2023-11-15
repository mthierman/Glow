#include <Windows.h>

#include "nlohmann/json.hpp"
#include "toml++/toml.hpp"

#include <string>
#include <sstream>
#include <filesystem>
#include <print>
#include <iostream>

#include "../filesystem/filesystem.hxx"
#include "../settings/settings.hxx"
#include "../text/text.hxx"

using json = nlohmann::json;
using std::println;
using namespace std::string_view_literals;

auto main() -> int
{
    settings s;
    json j = s;
    println("{}", j.dump());
    settings test = j;
    println("{}", test.name);

    static constexpr std::string_view some_toml = R"(
        [library]
        name = "toml++"
        cpp = 17
    )"sv;

    try
    {
        toml::table tbl = toml::parse(some_toml);
        std::cout << tbl << std::endl;
    }
    catch (const toml::parse_error& e)
    {
        println("{}", e.what());
    }

    return 0;
}
