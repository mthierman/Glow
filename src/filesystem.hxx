// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <ShlObj.h>

#include <filesystem>
#include <memory>
#include <print>
#include <string>

#include <wil/resource.h>

#include <sqlite3.h>

#include "console.hxx"

namespace glow
{
namespace filesystem
{
struct Database
{
    Database();
    ~Database();
    auto open() -> void;
    auto write() -> void;

  private:
    struct sqlite3_deleter
    {
        auto operator()(sqlite3* db) noexcept -> void { sqlite3_close(db); }
    };
    using sqlite3_ptr = std::unique_ptr<sqlite3, sqlite3_deleter>;
    sqlite3_ptr m_db;
    std::filesystem::path m_path;
};

auto known_folder(KNOWNFOLDERID folderId = FOLDERID_LocalAppData) -> std::filesystem::path;
auto app_name() -> std::string;
auto path_portable() -> std::filesystem::path;
auto path_portable_wide() -> std::filesystem::path;
} // namespace filesystem
} // namespace glow
