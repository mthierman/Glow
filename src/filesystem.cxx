// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/filesystem.hxx>
#include <glow/system.hxx>
#include <glow/text.hxx>

#include <wil/resource.h>
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
        throw std::overflow_error(glow::system::get_last_error());
    }

    buffer.resize(length - 2);

    auto tempFolder { std::filesystem::path(buffer) };

    for (const auto& subfolder : subfolders) {
        tempFolder /= subfolder;
    }

    return tempFolder;
}

auto file_watcher(std::filesystem::path path, std::function<void()> callback) -> void {
    auto handle { wil::unique_handle(
        ::CreateFileA(path.string().c_str(),
                      FILE_LIST_DIRECTORY,
                      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                      nullptr,
                      OPEN_EXISTING,
                      FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
                      nullptr)) };

    std::vector<::BYTE> buffer(0, 4096);
    ::DWORD bytes { 0 };
    ::OVERLAPPED overlapped;

    auto readDirectory { ::ReadDirectoryChangesW(
        handle.get(),
        buffer.data(),
        buffer.size(),
        FALSE,
        FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME,
        &bytes,
        &overlapped,
        nullptr) };
}
}; // namespace glow::filesystem
