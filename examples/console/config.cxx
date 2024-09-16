#include <Windows.h>

#include <stdlib.h>

#include <format>
#include <iostream>
#include <print>

#include <glow/glow.hxx>

auto main(/* int argc, char* argv[] */) -> int {
    glow::config::Config config;

    config(glow::filesystem::known_folder() / glow::text::String(u8"ini").wstring()
           / glow::text::String(u8"ini.json").wstring());

    config.save();

    config.load();

    config.print();

    return EXIT_SUCCESS;
}
