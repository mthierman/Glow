// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <ShlObj.h>

#include <expected>
#include <filesystem>
#include <format>
#include <initializer_list>
#include <string>

#include <glow/text.hxx>

namespace glow::filesystem {
auto known_folder(::KNOWNFOLDERID folderId = FOLDERID_LocalAppData)
    -> std::expected<std::filesystem::path, std::u8string>;

auto temp_folder() -> std::expected<std::filesystem::path, std::u8string>;

auto create_directory(const std::filesystem::path& path,
                      const std::filesystem::path& templatePath = "") -> bool;

auto move_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> bool;

auto copy_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> bool;

auto delete_file(const std::filesystem::path& path) -> bool;

auto create_symlink(const std::filesystem::path& target, const std::filesystem::path& destination)
    -> bool;

}; // namespace glow::filesystem

namespace std {
template <> struct formatter<std::filesystem::path> : formatter<string_view> {
    auto format(const std::filesystem::path& path, format_context& context) const noexcept {
        return formatter<string_view>::format(glow::text::c_str(path.u8string()), context);
    }
};

template <> struct formatter<std::filesystem::path, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::filesystem::path& path, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(glow::text::c_str(path.u16string()),
                                                        context);
    }
};
} // namespace std
