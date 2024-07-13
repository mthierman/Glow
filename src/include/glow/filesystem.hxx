// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <ShlObj.h>
#include <Windows.h>

#include <functional>
#include <filesystem>
#include <initializer_list>
#include <string>

namespace glow::filesystem {
auto known_folder(::KNOWNFOLDERID folderId = FOLDERID_LocalAppData,
                  std::initializer_list<std::string_view> subfolders = {}) -> std::filesystem::path;
auto temp_folder(std::initializer_list<std::string_view> subfolders = {}) -> std::filesystem::path;
auto file_watcher(std::filesystem::path path, std::function<void()> callback) -> void;
}; // namespace glow::filesystem
