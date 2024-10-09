// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/config.hxx>

#include <fstream>
#include <limits>
#include <sstream>

#include <glow/filesystem.hxx>
#include <glow/log.hxx>

// https://learn.microsoft.com/en-us/uwp/api/windows.data.json
// https://devblogs.microsoft.com/oldnewthing/20230102-00/?p=107632

namespace glow::config {
Config::Config(const std::filesystem::path& path)
    : path { path } {
    std::filesystem::create_directories(path.parent_path());
}

auto Config::serialize(const winrt::JsonObject& input) -> std::optional<std::u8string> {
    if (auto converted { glow::text::u8string(input.Stringify()) }; converted) {
        return converted.value();
    }

    return std::nullopt;
}

auto Config::deserialize(std::u8string_view input) -> std::optional<winrt::JsonObject> {
    winrt::JsonObject output;

    if (auto converted { glow::text::u16string(input) }; converted) {
        if (output.TryParse(glow::text::c_str(converted.value()), output)) {
            return output;
        }
    }

    return std::nullopt;
}

auto Config::save() -> bool {
    if (auto serialized { serialize(json) }; !path.empty() && serialized) {
        std::basic_ofstream<char8_t> file(
            path.c_str(), std::basic_ios<char8_t>::binary | std::basic_ios<char8_t>::out);

        if (file.write(serialized.value().c_str(), serialized.value().size())) {
            return true;
        }
    }

    return false;
}

auto Config::load() -> bool {
    if (auto file { std::basic_ifstream<char8_t>(
            path.c_str(), std::basic_ios<char8_t>::binary | std::basic_ios<char8_t>::in) };
        !path.empty() && file.is_open()) {
        file.ignore(std::numeric_limits<std::streamsize>::max());

        std::u8string buffer;
        buffer.resize(file.gcount());

        file.clear();
        file.seekg(0, std::basic_ios<char8_t>::beg);

        if (file.read(buffer.data(), buffer.size())) {
            if (auto deserialized { deserialize(buffer) }; deserialized) {
                json = deserialized.value();

                return true;
            }
        }
    }

    return false;
}
}; // namespace glow::config
