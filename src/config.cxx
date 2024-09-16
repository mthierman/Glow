// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/config.hxx>

#include <glow/filesystem.hxx>
#include <glow/text.hxx>

namespace glow::config {
auto Config::save() -> void {
    auto folder { glow::filesystem::known_folder() };
    auto dir { folder / glow::text::String(u8"ini").wstring()
               / glow::text::String(u8"ini.txt").wstring() };

    ::WritePrivateProfileStringW(L"TEST", L"TEST", L"TEST", dir.c_str());
}
}; // namespace glow::config
