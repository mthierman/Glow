#include <Windows.h>

#include <stdlib.h>

#include <format>
#include <iostream>
#include <print>

#include <glow/glow.hxx>

auto wmain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t* argv[]) -> int {
    if (glow::filesystem::known_folder().has_value()) {
        auto filePath { glow::filesystem::known_folder().value() / u"ini" / u"ini.json" };

        auto config { glow::config::Config(filePath) };

        config.set<std::u8string>(u8"u8string", u8"wrapper");
        config.set<bool>(u8"bool", true);
        config.set<double>(u8"number", 24);

        if (auto result { config.get<std::u8string>(u8"u8string") }; result.has_value()) {
            glow::log::log(result.value());
        }

        if (auto result { config.get<bool>(u8"bool") }; result.has_value()) {
            glow::log::log(result.value());
        }

        if (auto result { config.get<double>(u8"number") }; result.has_value()) {
            auto fmt { std::format("{}", result.value()) };
            glow::log::log(std::u8string(fmt.begin(), fmt.end()));
        }

        config.save();

        config.load();

        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
