// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/filesystem.hxx>

#include <wil/win32_helpers.h>

#include <glow/log.hxx>

namespace glow::filesystem {
auto create_directory(const std::filesystem::path& path, const std::filesystem::path& templatePath)
    -> bool {
    if (templatePath.empty()) {
        if (::CreateDirectoryW(path.c_str(), nullptr) != 0) {
            return true;
        } else {
            return false;
        }
    } else {
        if (::CreateDirectoryExW(templatePath.c_str(), path.c_str(), nullptr) != 0) {
            return true;
        } else {
            return false;
        }
    }
}

auto known_folder(::KNOWNFOLDERID folderId, std::initializer_list<std::u8string_view> subfolders)
    -> std::optional<std::filesystem::path> {
    wil::unique_cotaskmem_string buffer;

    if (::SHGetKnownFolderPath(folderId, 0, nullptr, &buffer) != S_OK) {
        return std::nullopt;
    }

    auto knownFolder { std::filesystem::path(buffer.get()) };

    for (auto subfolder : subfolders) {
        knownFolder /= glow::text::to_wstring(subfolder);
    }

    return knownFolder;
}

auto temp_folder(std::initializer_list<std::u8string_view> subfolders)
    -> std::optional<std::filesystem::path> {
    std::wstring buffer;
    auto length { ::GetTempPathW(0, buffer.data()) };
    buffer.resize(length);

    if (::GetTempPathW(length, buffer.data()) == 0) {
        return std::nullopt;
    }

    buffer.resize(length - 2);

    auto tempFolder { std::filesystem::path(buffer) };

    for (auto subfolder : subfolders) {
        tempFolder /= glow::text::to_wstring(subfolder);
    }

    return tempFolder;
}
}; // namespace glow::filesystem
