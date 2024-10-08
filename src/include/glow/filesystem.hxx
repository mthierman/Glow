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
#include <optional>
#include <string>

#include <glow/text.hxx>

namespace glow::filesystem {
auto create_directory(const std::filesystem::path& path,
                      const std::filesystem::path& templatePath = "") -> bool;
auto known_folder(::KNOWNFOLDERID folderId = FOLDERID_LocalAppData)
    -> std::optional<std::filesystem::path>;
auto temp_folder() -> std::optional<std::filesystem::path>;
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
