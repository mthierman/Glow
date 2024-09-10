// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/filesystem.hxx>
#include <glow/log.hxx>
#include <glow/system.hxx>
#include <glow/text.hxx>

#include <wil/win32_helpers.h>

namespace glow::filesystem {
auto known_folder(::KNOWNFOLDERID folderId,
                  std::initializer_list<std::string_view> subfolders) -> std::filesystem::path {
    wil::unique_cotaskmem_string buffer;
    ::SHGetKnownFolderPath(folderId, 0, nullptr, &buffer);

    auto knownFolder { std::filesystem::path(buffer.get()) };

    for (auto subfolder : subfolders) {
        knownFolder /= glow::text::to_wstring(subfolder);
    }

    return knownFolder;
}

auto temp_folder(std::initializer_list<std::string_view> subfolders) -> std::filesystem::path {
    std::wstring buffer;
    auto length { ::GetTempPathW(0, buffer.data()) };
    buffer.resize(length);

    if (::GetTempPathW(length, buffer.data()) == 0) {
        throw std::runtime_error(glow::log::get_last_error());
    }

    buffer.resize(length - 2);

    auto tempFolder { std::filesystem::path(buffer) };

    for (auto subfolder : subfolders) {
        tempFolder /= glow::text::to_wstring(subfolder);
    }

    return tempFolder;
}

auto to_string(const std::filesystem::path& path) -> std::string {
    return glow::text::to_string(path.c_str());
}
}; // namespace glow::filesystem
