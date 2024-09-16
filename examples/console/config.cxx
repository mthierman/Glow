#include <Windows.h>

#include <stdlib.h>

#include <format>
#include <iostream>
#include <print>

#include <glow/glow.hxx>

auto main(/* int argc, char* argv[] */) -> int {
    glow::config::Config config;

    config(glow::filesystem::known_folder() / glow::text::String(u8"ini").wstring()
           / glow::text::String(u8"ini.txt").wstring());

    config.save();

    return EXIT_SUCCESS;
}
