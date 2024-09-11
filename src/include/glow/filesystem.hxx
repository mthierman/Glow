// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <ShlObj.h>
#include <Windows.h>

#include <filesystem>
#include <format>
#include <initializer_list>
#include <string>

#include <glow/text.hxx>

namespace glow::filesystem {
auto known_folder(::KNOWNFOLDERID folderId = FOLDERID_LocalAppData,
                  std::initializer_list<std::string_view> subfolders = {}) -> std::filesystem::path;
auto temp_folder(std::initializer_list<std::string_view> subfolders = {}) -> std::filesystem::path;
auto to_string(const std::filesystem::path& path) -> std::string;
auto to_wstring(const std::filesystem::path& path) -> std::wstring;
}; // namespace glow::filesystem

namespace std {
template <> struct formatter<std::filesystem::path> : formatter<string_view> {
    auto format(const std::filesystem::path& path, format_context& context) const noexcept {
        return formatter<string_view>::format(glow::filesystem::to_string(path), context);
    }
};

template <> struct formatter<std::filesystem::path, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::filesystem::path& path, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(glow::filesystem::to_wstring(path), context);
    }
};
} // namespace std
