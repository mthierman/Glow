// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/config.hxx>

#include <glow/filesystem.hxx>
#include <glow/log.hxx>
#include <glow/text.hxx>

namespace glow::config {
auto Config::operator()(const std::filesystem::path& path) -> void {
    paths.file = path;
    paths.root = path.parent_path();
}

auto Config::save() -> void {
    winrt::JsonObject json;

    auto string { winrt::JsonValue::CreateStringValue(L"TestValue") };
    json.SetNamedValue(L"TestKey", string);

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
}

auto Config::load() -> void {
    //
}
}; // namespace glow::config
