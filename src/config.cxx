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
Config::Config(std::filesystem::path path)
    : path { std::move(path) } {
    std::filesystem::create_directories(this->path.parent_path());
}

auto Config::save() -> bool {
    if (auto converted { glow::text::u8string(json.Stringify()) }) {
        std::basic_ofstream<char8_t> file(
            path.c_str(), std::basic_ios<char8_t>::binary | std::basic_ios<char8_t>::out);

        if (file.write(converted->c_str(), converted->size())) {
            return true;
        }
    }

    return false;
}

auto Config::load() -> bool {
    if (auto file { std::basic_ifstream<char8_t>(
            path.c_str(), std::basic_ios<char8_t>::binary | std::basic_ios<char8_t>::in) };
        file.is_open()) {
        file.ignore(std::numeric_limits<std::streamsize>::max());

        std::u8string buffer;
        buffer.resize(file.gcount());

        file.clear();
        file.seekg(0, std::basic_ios<char8_t>::beg);

        if (file.read(buffer.data(), buffer.size())) {
            if (auto converted { glow::text::u16string(buffer) }) {
                if (json.TryParse(glow::text::c_str(*converted), json)) {
                    return true;
                }
            }
        }
    }

    return false;
}
}; // namespace glow::config
