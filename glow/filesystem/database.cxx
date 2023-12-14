// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <filesystem/database.hxx>

//==============================================================================
namespace glow::filesystem
{

//==============================================================================
Database::Database() {}

Database::~Database() {}

//==============================================================================
auto Database::open() -> void
{
    if (sqlite3_open(path.string().c_str(), std::out_ptr(p_db)) != SQLITE_OK)
        throw std::runtime_error("Failed to open SQLite");
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
        std::string error;

        auto dbExec{sqlite3_exec(p_db.get(), sql.c_str(), nullptr, 0, std::out_ptr(error))};

        if (dbExec != SQLITE_OK)
        {
            std::println("{}", error);
            sqlite3_free(error.data());
        }
    }
}

//==============================================================================
} // namespace glow::filesystem
