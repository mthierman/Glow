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
    Path(const Path& path) = default;
    Path(Path&& path) = default;
    explicit Path(const std::filesystem::path& path);
    explicit Path(std::filesystem::path&& path);
    explicit Path(const std::u8string& u8string);

    auto operator=(const Path& path) -> Path& = default;
    auto operator=(Path&& path) -> Path& = default;

    auto operator()(const std::u8string& string) -> const std::u8string&;
    auto operator()(std::u8string&& string) -> const std::u8string&;
    auto operator()() const -> const std::u8string&;

    auto u8string() const -> std::u8string;
    auto u16string() const -> std::u16string;
    auto string() const -> std::string;
    auto wstring() const -> std::wstring;

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
