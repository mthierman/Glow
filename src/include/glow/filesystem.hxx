// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <ShlObj.h>

#include <filesystem>
#include <format>
#include <initializer_list>
#include <string>

#include <glow/text.hxx>

namespace glow::filesystem {
auto known_folder(::KNOWNFOLDERID folderId = FOLDERID_LocalAppData,
                  std::initializer_list<std::u8string_view> subfolders
                  = {}) -> std::filesystem::path;
auto temp_folder(std::initializer_list<std::u8string_view> subfolders
                 = {}) -> std::filesystem::path;

struct Path {
    Path() = default;
    ~Path() = default;

protected:
    std::filesystem::path storage;
};
}; // namespace glow::filesystem

namespace std {
template <> struct formatter<std::filesystem::path> : formatter<string_view> {
    auto format(const std::filesystem::path& path, format_context& context) const noexcept {
        return formatter<string_view>::format(glow::text::to_string(path.u8string()), context);
    }
};

template <> struct formatter<std::filesystem::path, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::filesystem::path& path, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(glow::text::to_wstring(path.u8string()),
                                                        context);
    }
};
} // namespace std
