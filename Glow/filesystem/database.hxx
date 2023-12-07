// clang-format off
// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <filesystem>
#include <memory>
#include <print>
#include <stdexcept>
#include <string>

#include <sqlite3.h>

#include <filesystem/filesystem.hxx>

//==============================================================================
namespace glow::filesystem
{
struct Database
{
    Database();
    ~Database();

    //==============================================================================
    auto open() -> void;
    auto write() -> void;

  private:
    struct sqlite3_deleter
    {
        auto operator()(sqlite3* db) noexcept -> void { sqlite3_close(db); }
    };

    //==============================================================================
    using sqlite3_ptr = std::unique_ptr<sqlite3, sqlite3_deleter>;
    sqlite3_ptr db;

    //==============================================================================
    std::filesystem::path path{(get_pgmptr() / "db.sqlite")};
};
} // namespace glow::filesystem
