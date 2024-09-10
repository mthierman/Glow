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

    auto knownFolder { std::filesystem::path(glow::text::utf16_to_utf8(buffer.get())) };

    for (const auto& subfolder : subfolders) {
        knownFolder /= subfolder;
    }

    return knownFolder;
}

auto temp_folder(std::initializer_list<std::string_view> subfolders) -> std::filesystem::path {
    std::string buffer;
    auto length { ::GetTempPathA(0, buffer.data()) };
    buffer.resize(length);

    if (::GetTempPathA(length, buffer.data()) == 0) {
        throw std::overflow_error(glow::log::get_last_error());
    }

    buffer.resize(length - 2);

    auto tempFolder { std::filesystem::path(buffer) };

    for (const auto& subfolder : subfolders) {
        tempFolder /= subfolder;
    }

    return tempFolder;
}
}; // namespace glow::filesystem
