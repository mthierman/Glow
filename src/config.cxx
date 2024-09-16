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
Config::Config(const std::filesystem::path& path) { file_path(path); }

auto Config::file_path(const std::filesystem::path& path) -> void {
    paths.file = path;
    paths.root = path.parent_path();
}

auto Config::save() -> void {
    std::basic_ofstream<char8_t> file(
        paths.file.c_str(), std::basic_ios<char8_t>::binary | std::basic_ios<char8_t>::out);
    auto stringify { glow::text::to_u8string(json.Stringify()) };
    file.write(stringify.c_str(), stringify.size());
}

auto Config::load() -> void {
    std::basic_ifstream<char8_t> file(
        paths.file.c_str(), std::basic_ios<char8_t>::binary | std::basic_ios<char8_t>::in);

    if (file.is_open()) {
        std::u8string buffer;

        file.ignore(std::numeric_limits<std::streamsize>::max());
        buffer.resize(file.gcount());
        glow::log::log("{}", file.gcount());
        file.clear();
        file.seekg(0, std::basic_ios<char8_t>::beg);
        file.read(buffer.data(), buffer.size());

        auto parse { json.TryParse(glow::text::to_wstring(buffer), json) };

        if (parse) {
            glow::log::log("parsed!");
        }
    }
}
}; // namespace glow::config
