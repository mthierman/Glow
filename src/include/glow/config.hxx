// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <filesystem>
#include <optional>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.Data.Json.h>

#include <glow/text.hxx>

namespace winrt {
using namespace winrt::Windows::Data::Json;
}; // namespace winrt

namespace glow::config {
struct Config {
    Config() = delete;
    virtual ~Config() = default;
    Config(const Config& config) = default;
    Config(Config&& config) = default;

    explicit Config(const std::filesystem::path& path);

    auto operator=(const Config& config) -> Config& = default;
    auto operator=(Config&& config) -> Config& = default;

    template <typename T, typename U> auto set(std::u8string_view key, U value) -> void {
        if constexpr (std::is_same_v<T, std::u8string>) {
            json.SetNamedValue(glow::text::to_wstring(key),
                               winrt::JsonValue::CreateStringValue(glow::text::to_wstring(value)));
        }

        if constexpr (std::is_same_v<T, bool>) {
            json.SetNamedValue(glow::text::to_wstring(key),
                               winrt::JsonValue::CreateBooleanValue(value));
        }

        if constexpr (std::is_same_v<T, double>) {
            json.SetNamedValue(glow::text::to_wstring(key),
                               winrt::JsonValue::CreateNumberValue(value));
        }
    }

    template <typename T> auto get(std::u8string_view key) -> T {
        auto value { json.GetNamedValue(glow::text::to_wstring(key), nullptr) };

        if constexpr (std::is_same_v<T, std::u8string>) {
            if (value && value.ValueType() == winrt::JsonValueType::String) {
                return glow::text::to_u8string(value.GetString());
            } else {
                throw std::runtime_error("Config key doesn't exist");
            }
        }

        if constexpr (std::is_same_v<T, bool>) {
            if (value && value.ValueType() == winrt::JsonValueType::Boolean) {
                return value.GetBoolean();
            } else {
                throw std::runtime_error("Config key doesn't exist");
            }
        }

        if constexpr (std::is_same_v<T, double>) {
            if (value && value.ValueType() == winrt::JsonValueType::Number) {
                return value.GetNumber();
            } else {
                throw std::runtime_error("Config key doesn't exist");
            }
        }
    }

    auto serialize() -> std::u8string;
    auto deserialize(std::u8string_view input) -> std::optional<winrt::JsonObject>;

    auto save() -> void;
    auto load() -> bool;

protected:
    std::filesystem::path path;
    winrt::JsonObject json;
};
}; // namespace glow::config
