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
    virtual ~Config() = default;

    Config(const Config& config) = default;
    auto operator=(const Config& config) -> Config& = default;

    Config(Config&& config) = default;
    auto operator=(Config&& config) -> Config& = default;

    explicit Config(std::filesystem::path path);

    auto save() -> bool;
    auto load() -> bool;

    template <typename T, typename U> auto set(std::u8string_view key, U value) -> void {
        if constexpr (std::is_same_v<T, std::u8string>) {
            if (auto convertedKey { glow::text::u16string(key) }) {
                if (auto convertedValue { glow::text::u16string(value) }) {
                    json.SetNamedValue(
                        glow::text::c_str(*convertedKey),
                        winrt::JsonValue::CreateStringValue(glow::text::c_str(*convertedValue)));
                }
            }
        }

        if constexpr (std::is_same_v<T, bool>) {
            if (auto convertedKey { glow::text::u16string(key) }) {
                json.SetNamedValue(glow::text::c_str(*convertedKey),
                                   winrt::JsonValue::CreateBooleanValue(value));
            }
        }

        if constexpr (std::is_same_v<T, double>) {
            if (auto convertedKey { glow::text::u16string(key) }) {
                json.SetNamedValue(glow::text::c_str(*convertedKey),
                                   winrt::JsonValue::CreateNumberValue(value));
            }
        }
    }

    template <typename T> auto get(std::u8string_view key) -> std::optional<T> {
        if (auto convertedKey { glow::text::u16string(key) }) {
            auto value { json.GetNamedValue(glow::text::c_str(*convertedKey), nullptr) };

            if constexpr (std::is_same_v<T, std::u8string>) {
                if (value && value.ValueType() == winrt::JsonValueType::String) {
                    if (auto convertedValue { glow::text::u8string(value.GetString()) }) {
                        return *convertedValue;
                    }
                }
            }

            if constexpr (std::is_same_v<T, bool>) {
                if (value && value.ValueType() == winrt::JsonValueType::Boolean) {
                    return value.GetBoolean();
                }
            }

            if constexpr (std::is_same_v<T, double>) {
                if (value && value.ValueType() == winrt::JsonValueType::Number) {
                    return value.GetNumber();
                }
            }
        }

        return std::nullopt;
    }

    std::filesystem::path path;
    winrt::JsonObject json;
};
}; // namespace glow::config
