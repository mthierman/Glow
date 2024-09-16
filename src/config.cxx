// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/config.hxx>

#include <fstream>
#include <sstream>

#include <glow/filesystem.hxx>
#include <glow/log.hxx>
#include <glow/text.hxx>

namespace glow::config {
Config::Config() {
    // auto value1 { winrt::JsonValue::CreateStringValue(L"Value1") };
    // auto value2 { winrt::JsonValue::CreateStringValue(L"Value2") };
    // json.SetNamedValue(L"Key1", value1);
    // json.SetNamedValue(L"Key2", value2);
}

auto Config::operator()(const std::filesystem::path& path) -> void {
    paths.file = path;
    paths.root = path.parent_path();
}

// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
// https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
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
        file.seekg(0, std::basic_ios<char8_t>::end);
        buffer.resize(file.tellg());
        file.seekg(0, std::basic_ios<char8_t>::beg);
        file.read(buffer.data(), buffer.size());
        auto parse { json.TryParse(glow::text::to_wstring(buffer), json) };

        if (parse) {
            glow::log::log("parsed!");
        }
    }
}

auto Config::load_stringstream() -> void {
    std::basic_ifstream<char8_t> file(
        paths.file, std::basic_ios<char8_t>::binary | std::basic_ios<char8_t>::in);

    if (file.is_open()) {
        std::basic_ostringstream<char8_t> buffer;
        buffer << file.rdbuf();
        auto parse { json.TryParse(glow::text::to_wstring(buffer.str()), json) };

        if (parse) {
            glow::log::log("parsed!");
        }
    }
}

auto Config::print() -> void {
    if (auto value { json.GetNamedValue(L"Key1", nullptr) };
        value && value.ValueType() == winrt::JsonValueType::String) {
        auto name { value.GetString() };
        glow::log::log("{}", glow::text::to_string(name));
    }

    if (auto value { json.GetNamedValue(L"Key2", nullptr) };
        value && value.ValueType() == winrt::JsonValueType::String) {
        auto name { value.GetString() };
        glow::log::log("{}", glow::text::to_string(name));
    }
}
}; // namespace glow::config
