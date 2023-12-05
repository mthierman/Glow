// clang-format off
// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝
// clang-format on

#include <filesystem/database.hxx>

//==============================================================================
namespace glow::filesystem
{
Database::Database()
{
    if (sqlite3_open(path.string().c_str(), &buffer) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to open SQLite");
    }

    db = sqlite3_ptr(buffer);
}

Database::~Database() {}

//==============================================================================
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
        std::string string;
        auto error{string.data()};

        auto debExec{sqlite3_exec(db.get(), sql.c_str(), nullptr, 0, &error)};

        if (debExec != SQLITE_OK)
        {
            std::println("{}", error);
            sqlite3_free(error);
        }
    }
}
} // namespace glow::filesystem