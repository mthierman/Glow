// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/filesystem.hxx>

#include <wil/win32_helpers.h>

#include <glow/system.hxx>

namespace glow::filesystem {
auto known_folder(::KNOWNFOLDERID folderId) -> std::expected<std::filesystem::path, std::u8string> {
    wil::unique_cotaskmem_string buffer;

    if (auto hr { ::SHGetKnownFolderPath(folderId, 0, nullptr, &buffer) }; hr == S_OK) {
        return buffer.get();
    } else {
        return std::unexpected(glow::system::format_message(hr));
    }
}

auto temp_folder() -> std::expected<std::filesystem::path, std::u8string> {
    std::wstring buffer;
    buffer.resize(::GetTempPathW(0, buffer.data()));

    if (::GetTempPathW(static_cast<::DWORD>(buffer.size()), buffer.data()) != 0) {
        buffer.resize(buffer.size() - 2);

        return buffer;
    } else {
        return std::unexpected(glow::system::get_last_error());
    }
}

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

auto move_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> bool {
    if (::MoveFileW(origin.c_str(), destination.c_str()) != 0) {
        return true;
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

auto delete_file(const std::filesystem::path& path) -> bool {
    if (::DeleteFileW(path.c_str()) != 0) {
        return true;
    }

    return false;
}

auto create_symlink(const std::filesystem::path& target, const std::filesystem::path& destination)
    -> bool {
    auto flags { std::filesystem::is_directory(target)
                     ? SYMBOLIC_LINK_FLAG_DIRECTORY | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
                     : SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE };

    if (::CreateSymbolicLinkW(destination.c_str(), target.c_str(), static_cast<::DWORD>(flags))
        != 0) {
        return true;
    }

    return false;
}
}; // namespace glow::filesystem
