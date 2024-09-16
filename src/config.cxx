// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/config.hxx>

#include <glow/filesystem.hxx>
#include <glow/text.hxx>

namespace glow::config {
auto Config::operator()(const std::filesystem::path& path) -> void { this->path = path; }

auto Config::save() -> void {
    ::WritePrivateProfileStringW(L"TEST", L"TEST", L"TEST", path.c_str());
}
}; // namespace glow::config
