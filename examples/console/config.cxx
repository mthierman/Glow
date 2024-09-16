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

    auto value { config.get<std::u8string>(u8"string") };

    glow::log::log("{}", glow::text::to_string(value));

    config.save();

    // config.load();

    // config.print();

    return EXIT_SUCCESS;
}
