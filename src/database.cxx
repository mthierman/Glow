// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "database.hxx"
#include "filesystem.hxx"
#include <Windows.h>
#include <print>
#include <string>

namespace glow
{
Database::Database() : path{glow::app_path() / "db.sqlite"} {}

Database::~Database() {}

auto Database::open() -> void
{
    if (sqlite3_open(path.string().c_str(), std::out_ptr(db)) != SQLITE_OK)
        throw std::runtime_error("SQLite open failure");
}

auto Database::write() -> void
{
    std::string sql{"CREATE TABLE CONFIG("
                    "X INT NOT NULL,"
                    "Y INT NOT NULL,"
                    "WIDTH INT NOT NULL,"
                    "HEIGHT INT NOT NULL,"
                    "MENU INT NOT NULL,"
                    "SPLIT INT NOT NULL,"
                    "MAXIMIZED INT NOT NULL,"
                    "FULLSCREEN INT NOT NULL,"
                    "TOPMOST INT NOT NULL,"
                    "MAIN TEXT NOT NULL,"
                    "SIDE TEXT NOT NULL);"};

    if (std::filesystem::exists(path))
    {
        std::string error{};

        auto dbExec{sqlite3_exec(db.get(), sql.c_str(), nullptr, 0, std::out_ptr(error))};

        if (dbExec != SQLITE_OK)
        {
            std::println("{}", error);
            sqlite3_free(error.data());
        }
    }
}
} // namespace glow
