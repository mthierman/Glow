#include <GlowConfig.h>

#include <Windows.h>

#include <filesystem>
#include <iostream>
#include <print>
#include <sstream>
#include <string>

#include <nlohmann/json.hpp>
#include <toml++/toml.hpp>

#include <filesystem/filesystem.hxx>
#include <settings/settings.hxx>
#include <text/text.hxx>

using json = nlohmann::json;
using std::println;
using namespace std::string_view_literals;

struct settings
{
    std::string name;
    std::string version;
    int width{0};
    int height{0};
};

void to_json(json& j, const settings& s)
{
    j = json{{"name", s.name}, {"version", s.version}, {"width", s.width}, {"height", s.height}};
}

void from_json(const json& j, settings& s)
{
    j.at("name").get_to(s.name);
    j.at("version").get_to(s.version);
    j.at("width").get_to(s.width);
    j.at("height").get_to(s.height);
}

auto main() -> int
{
    std::println("{}", glow::filesystem::known_folder(FOLDERID_LocalAppData).string());
    std::println("{}", glow::filesystem::path_portable().string());
    std::println("{}", glow::text::randomize("Randomized!"));

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
