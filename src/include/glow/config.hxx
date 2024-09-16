// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <filesystem>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.Data.Json.h>

namespace winrt {
using namespace winrt::Windows::Data::Json;
}; // namespace winrt

namespace glow::config {
struct Config {
    Config() = default;

    auto operator()(const std::filesystem::path& path) -> void;

    auto save() -> void;
    auto load() -> void;

protected:
    struct Paths {
        std::filesystem::path root;
        std::filesystem::path file;
    };
    Paths paths;

    winrt::JsonObject json;
};
}; // namespace glow::config
