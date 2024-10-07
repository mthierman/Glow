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
struct Config final {
    Config() = delete;
    ~Config() = default;
    Config(const Config& config) = default;
    Config(Config&& config) = default;

    explicit Config(const std::filesystem::path& path);

    auto operator=(const Config& config) -> Config& = default;
    auto operator=(Config&& config) -> Config& = default;

    template <typename T, typename U> auto set(std::u8string_view key, U value) -> void {
        if constexpr (std::is_same_v<T, std::u8string>) {
            auto convertedKey { glow::text::convert(key) };
            auto convertedValue { glow::text::convert(value) };

            if (!convertedKey.has_value() || !convertedValue.has_value()) {
                return false;
            }

            json.SetNamedValue(
                { convertedKey.value().begin(), convertedKey.value().end() },
                winrt::JsonValue::CreateStringValue(
                    { convertedValue.value().begin(), convertedValue.value().end() }));
        }

        if constexpr (std::is_same_v<T, bool>) {
            auto convertedKey { glow::text::convert(key) };

            if (!convertedKey.has_value()) {
                return false;
            }

            json.SetNamedValue({ convertedKey.value().begin(), convertedKey.value().end() },
                               winrt::JsonValue::CreateBooleanValue(value));
        }

        if constexpr (std::is_same_v<T, double>) {
            auto convertedKey { glow::text::convert(key) };

            if (!convertedKey.has_value()) {
                return false;
            }

            json.SetNamedValue({ convertedKey.value().begin(), convertedKey.value().end() },
                               winrt::JsonValue::CreateNumberValue(value));
        }
    }

    template <typename T> auto get(std::u8string_view key) -> std::optional<T> {
        auto convertedKey { glow::text::convert(key) };

        if (!convertedKey.has_value()) {
            return std::nullopt;
        }

        auto value { json.GetNamedValue(
            { convertedKey.value().begin(), convertedKey.value().end() }, nullptr) };

        if constexpr (std::is_same_v<T, std::u8string>) {
            if (value && value.ValueType() == winrt::JsonValueType::String) {
                auto stringValue { value.GetString() };
                auto converted { glow::text::convert(stringValue.begin(), stringValue.end()) };
                if (!converted.has_value()) {
                    return std::nullopt;
                }

                return converted.value();
            } else {
                return std::nullopt;
            }
        }

        if constexpr (std::is_same_v<T, bool>) {
            if (value && value.ValueType() == winrt::JsonValueType::Boolean) {
                return value.GetBoolean();
            } else {
                return std::nullopt;
            }
        }

        if constexpr (std::is_same_v<T, double>) {
            if (value && value.ValueType() == winrt::JsonValueType::Number) {
                return value.GetNumber();
            } else {
                return std::nullopt;
            }
        }
    }

    auto serialize(const winrt::JsonObject& input) -> std::optional<std::u8string>;
    auto deserialize(std::u8string_view input) -> std::optional<winrt::JsonObject>;

    auto save() -> bool;
    auto load() -> bool;

protected:
    std::filesystem::path path;
    winrt::JsonObject json;
};
}; // namespace glow::config
