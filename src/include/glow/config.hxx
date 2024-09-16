// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <filesystem>

namespace glow::config {
struct Config {
    Config() = default;

    auto operator()(const std::filesystem::path& path) -> void;

    auto save() -> void;

protected:
    std::filesystem::path path;
};
}; // namespace glow::config
