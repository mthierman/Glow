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

  private:
    struct sqlite3_deleter
    {
        void operator()(sqlite3* db) { sqlite3_close(db); }
    };
    using sqlite3_ptr = std::unique_ptr<sqlite3, sqlite3_deleter>;
    sqlite3* db{nullptr};

  public:
    auto initialize(const std::filesystem::path& path) -> sqlite3_ptr;
    auto write() -> void;
};
} // namespace glow::filesystem
