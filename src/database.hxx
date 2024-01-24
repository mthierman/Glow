// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <wil/resource.h>
#include <filesystem>
#include <memory>
#include <print>
#include <string>
#include <sqlite3.h>

#include "filesystem.hxx"

namespace glow
{
namespace database
{
struct Database
{
    struct sqlite3_deleter
    {
        auto operator()(sqlite3* db) noexcept -> void { sqlite3_close(db); }
    };

    Database();
    ~Database();

    auto open() -> void;
    auto write() -> void;

  private:
    using sqlite3_ptr = std::unique_ptr<sqlite3, sqlite3_deleter>;
    sqlite3_ptr m_db{};
    std::filesystem::path m_path{glow::filesystem::path_portable() / "db.sqlite"};
};
} // namespace database
} // namespace glow
