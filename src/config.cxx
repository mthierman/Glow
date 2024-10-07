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

auto Config::serialize(const winrt::JsonObject& input) -> std::u8string {
    return glow::text::to_u8string(input.Stringify());
}

auto Config::deserialize(std::u8string_view input) -> std::optional<winrt::JsonObject> {
    winrt::JsonObject output;

    if (output.TryParse(glow::text::to_wstring(input), output)) {
        return output;
    } else {
        return std::nullopt;
    }
}

auto Config::save() -> bool {
    if (path.empty()) {
        return false;
    } else {
        auto serialized { serialize(json) };

        std::basic_ofstream<char8_t> file(
            path.c_str(), std::basic_ios<char8_t>::binary | std::basic_ios<char8_t>::out);

        if (file.write(serialized.c_str(), serialized.size())) {
            return true;
        } else {
            return false;
        }
    }
}

auto Config::load() -> bool {
    std::basic_ifstream<char8_t> file(
        path.c_str(), std::basic_ios<char8_t>::binary | std::basic_ios<char8_t>::in);

    if (file.is_open()) {
        std::u8string buffer;

        file.ignore(std::numeric_limits<std::streamsize>::max());
        buffer.resize(file.gcount());
        file.clear();
        file.seekg(0, std::basic_ios<char8_t>::beg);
        if (file.read(buffer.data(), buffer.size())) {
            if (deserialize(buffer).has_value()) {
                return true;
            }
        }
    } else {
        return false;
    }
}
}; // namespace glow::config
