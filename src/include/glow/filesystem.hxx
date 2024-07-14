// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <ShlObj.h>
#include <Windows.h>

#include <filesystem>
#include <functional>
#include <initializer_list>
#include <string>

#include <wil/resource.h>

namespace glow::filesystem {
auto known_folder(::KNOWNFOLDERID folderId = FOLDERID_LocalAppData,
                  std::initializer_list<std::string_view> subfolders = {}) -> std::filesystem::path;
auto temp_folder(std::initializer_list<std::string_view> subfolders = {}) -> std::filesystem::path;

// https://qualapps.blogspot.com/2010/05/understanding-readdirectorychangesw.html
// https://qualapps.blogspot.com/2010/05/understanding-readdirectorychangesw_19.html
auto CALLBACK file_io_completion_routine(::DWORD errorCode,
                                         ::DWORD numberOfBytesTransfered,
                                         ::LPOVERLAPPED overlapped) -> void;
struct Watcher {
    Watcher(std::filesystem::path path, std::function<void()> callback = []() {});
    auto readDirectoryChanges() -> bool;

    std::filesystem::path m_path;
    wil::unique_handle m_handle;
    std::function<void()> m_callback;
    std::vector<::BYTE> m_buffer;
    // FILE_NOTIFY_INFORMATION m_buffer {};

    ::DWORD m_bytes { 0 };
    ::OVERLAPPED m_overlapped { .Internal { 0 }, .InternalHigh { 0 }, .hEvent { this } };
};
}; // namespace glow::filesystem
