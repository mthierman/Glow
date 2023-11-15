#pragma once

#include "nlohmann/json.hpp"

using json = nlohmann::json;

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
