#include <Windows.h>

#include <stdlib.h>

#include <format>
#include <iostream>
#include <print>

#include <glow/glow.hxx>

auto main(/* int argc, char* argv[] */) -> int {
    auto filePath { glow::filesystem::known_folder() / glow::text::String(u8"ini").wstring()
                    / glow::text::String(u8"ini.json").wstring() };

    auto config { glow::config::Config(filePath) };

    config.set<std::u8string>(u8"string", u8"wrapper");
    config.set<bool>(u8"bool", true);
    config.set<double>(u8"number", 24);

    // auto value { config.get<std::u8string>(u8"string") };
    // auto value { config.get<std::u8string>(u8"string") };
    // auto value { config.get<std::u8string>(u8"string") };
    // glow::log::log("{}", glow::text::to_string(value));

    glow::log::log("{}, {}, {}",
                   config.get<std::u8string>(u8"string"),
                   config.get<bool>(u8"bool"),
                   config.get<double>(u8"number"));

    config.save();

    // config.load();

    return EXIT_SUCCESS;
}
