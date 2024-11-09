// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <filesystem>
#include <string>

namespace glow::network {
auto download_file(std::string_view url, const std::filesystem::path& path) -> bool;
};
