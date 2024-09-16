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
auto Config::operator()(const std::filesystem::path& path) -> void {
    paths.file = path;
    paths.root = path.parent_path();
}

// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
auto Config::save() -> void {
    auto testValue1 { winrt::JsonValue::CreateStringValue(L"TestValue1") };
    auto testValue2 { winrt::JsonValue::CreateStringValue(L"TestValue2") };
    json.SetNamedValue(L"TestKey", testValue1);
    json.SetNamedValue(L"TestKey2", testValue2);

    // if (json.HasKey(L"TestKey")) {
    //     auto value { json.GetNamedValue(L"TestKey") };
    //     if (value.ValueType() == winrt::JsonValueType::String) {
    //         auto name = value.GetString();
    //         glow::log::log("{}", glow::text::to_string(name));
    //     }
    // }

    auto value = json.GetNamedValue(L"TestKey", nullptr);
    if (value && value.ValueType() == winrt::JsonValueType::String) {
        auto name { value.GetString() };
        glow::log::log("{}", glow::text::to_string(name));
    }

    glow::log::log("{}", glow::text::to_string(json.Stringify()));
    glow::log::log("{}", glow::text::to_string(json.ToString()));

    std::ofstream f(paths.file);
    f << glow::text::to_string(json.ToString()) << std::endl;
}

auto Config::load() -> void {
    std::ifstream f(paths.file);
    std::stringstream buffer;
    buffer << f.rdbuf();

    auto parse = json.TryParse(glow::text::to_wstring(buffer.str()), json);
    if (parse) {
        glow::log::log("parsed!");
    }

    auto value = json.GetNamedValue(L"TestKey", nullptr);
    if (value && value.ValueType() == winrt::JsonValueType::String) {
        auto name { value.GetString() };
        glow::log::log("{}", glow::text::to_string(name));
    }

    glow::log::log("{}", buffer.str());
}
}; // namespace glow::config
