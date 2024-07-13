// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/filesystem.hxx>
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
        throw std::overflow_error(glow::system::get_last_error());
    }

    buffer.resize(length - 2);

    auto tempFolder { std::filesystem::path(buffer) };

    for (const auto& subfolder : subfolders) {
        tempFolder /= subfolder;
    }

    return tempFolder;
}

auto CALLBACK file_io_completion_routine(::DWORD errorCode,
                                         ::DWORD numberOfBytesTransfered,
                                         ::OVERLAPPED* overlapped) -> void {
    auto self { static_cast<Watcher*>(overlapped->hEvent) };
    glow::system::dbg("{}", self->m_path);
    self->m_callback();
    self->readDirectoryChanges();
}

Watcher::Watcher(std::filesystem::path path, std::function<void()> callback)
    : m_path { path },
      m_handle { ::CreateFileA(path.string().c_str(),
                               FILE_LIST_DIRECTORY,
                               FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                               nullptr,
                               OPEN_EXISTING,
                               FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
                               nullptr) },
      m_callback { callback },
      m_buffer(4096) { }

auto Watcher::readDirectoryChanges() -> bool {
    return ::ReadDirectoryChangesW(m_handle.get(),
                                   m_buffer.data(),
                                   static_cast<::DWORD>(m_buffer.size()),
                                   FALSE,
                                   FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION
                                       | FILE_NOTIFY_CHANGE_FILE_NAME,
                                   &m_bytes,
                                   &m_overlapped,
                                   file_io_completion_routine);
}
}; // namespace glow::filesystem
