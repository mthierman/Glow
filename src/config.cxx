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
    if (auto converted {
            glow::text::u8string(reinterpret_cast<const char16_t*>(input.Stringify().data())) };
        converted.has_value()) {
        return converted.value();
    }

    return std::nullopt;
}

auto Config::deserialize(std::u8string_view input) -> std::optional<winrt::JsonObject> {
    winrt::JsonObject output;

    if (auto converted { glow::text::u16string(input) }; converted.has_value()) {
        if (output.TryParse(
                std::wstring(converted.value().begin(), converted.value().end()).c_str(), output)) {
            return output;
        }
    }

    return std::nullopt;
}

auto Config::save() -> bool {
    if (path.empty()) {
        return false;
    }

    auto serialized { serialize(json) };

    if (!serialized.has_value()) {
        return false;
    }

    std::basic_ofstream<char8_t> file(
        path.c_str(), std::basic_ios<char8_t>::binary | std::basic_ios<char8_t>::out);

    if (file.write(serialized.value().c_str(), serialized.value().size())) {
        return true;
    }

    return false;
}

auto Config::load() -> bool {
    if (path.empty()) {
        return false;
    }

    std::basic_ifstream<char8_t> file(
        path.c_str(), std::basic_ios<char8_t>::binary | std::basic_ios<char8_t>::in);

    if (!file.is_open()) {
        return false;
    }

    file.ignore(std::numeric_limits<std::streamsize>::max());

    std::u8string buffer;
    buffer.resize(file.gcount());

    file.clear();
    file.seekg(0, std::basic_ios<char8_t>::beg);

    if (!file.read(buffer.data(), buffer.size())) {
        return false;
    }

    if (auto deserialized { deserialize(buffer) }; deserialized.has_value()) {
        json = deserialized.value();

        return true;
    }

    return false;
}
}; // namespace glow::config
