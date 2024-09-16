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
    ::WritePrivateProfileStringW(
        L"TEST", L"TEST", L"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ", paths.file.c_str());
}

auto Config::load() -> void {
    // ::WritePrivateProfileStringW(L"TEST", L"TEST", L"TEST", paths.file.c_str());
    std::wstring buffer;
    auto length { ::GetPrivateProfileStringW(
        L"TEST", L"TEST", L"TEST", buffer.data(), 0, paths.file.c_str()) };
    glow::log::log("length: {}", length);
    buffer.resize(length);
    ::GetPrivateProfileStringW(
        L"TEST", L"TEST", L"TEST", buffer.data(), length, paths.file.c_str());

    glow::log::log(L"{}", buffer);
}
}; // namespace glow::config
