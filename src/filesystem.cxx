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
        }
    } else {
        if (::CreateDirectoryExW(templatePath.c_str(), path.c_str(), nullptr) != 0) {
            return true;
        }
    }

    return false;
}

auto copy_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> bool {
    if (::CopyFile2(origin.c_str(), destination.c_str(), nullptr) == S_OK) {
        return true;
    }

    return false;
}

auto create_symlink(const std::filesystem::path& target, const std::filesystem::path& destination)
    -> bool {
    ::DWORD flags { std::filesystem::is_directory(target)
                        ? SYMBOLIC_LINK_FLAG_DIRECTORY
                            | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
                        : SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE };

    if (::CreateSymbolicLinkW(destination.c_str(), target.c_str(), flags) != 0) {
        return true;
    }

    return false;
}

auto known_folder(::KNOWNFOLDERID folderId) -> std::optional<std::filesystem::path> {
    wil::unique_cotaskmem_string buffer;

    if (::SHGetKnownFolderPath(folderId, 0, nullptr, &buffer) == S_OK) {
        return buffer.get();
    }

    return std::nullopt;
}

auto temp_folder() -> std::optional<std::filesystem::path> {
    std::wstring buffer;
    buffer.resize(::GetTempPathW(0, buffer.data()));

    if (::GetTempPathW(static_cast<::DWORD>(buffer.size()), buffer.data()) != 0) {
        buffer.resize(buffer.size() - 2);

        return buffer;
    }

    return std::nullopt;
}
}; // namespace glow::filesystem
