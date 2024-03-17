// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <filesystem>
#include <memory>
#include <sqlite3.h>

namespace glow
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
    sqlite3_ptr db{};
    std::filesystem::path path{};
};
} // namespace glow
