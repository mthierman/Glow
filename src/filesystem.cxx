// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "filesystem.hxx"

namespace glow::filesystem
{
Database::Database() { m_path = (path_portable() / "db.sqlite"); }

Database::~Database() {}

auto Database::open() -> void
{
    if (sqlite3_open(m_path.string().c_str(), std::out_ptr(m_db)) != SQLITE_OK)
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

    if (std::filesystem::exists(m_path))
    {
        std::string error;

        auto dbExec{sqlite3_exec(m_db.get(), sql.c_str(), nullptr, 0, std::out_ptr(error))};

        if (dbExec != SQLITE_OK)
        {
            std::println("{}", error);
            sqlite3_free(error.data());
        }
    }
}

auto known_folder(KNOWNFOLDERID folderId) -> std::filesystem::path
{
    // https://learn.microsoft.com/en-us/windows/win32/shell/knownfolderid
    wil::unique_cotaskmem_string buffer;

    glow::console::hresult_check(SHGetKnownFolderPath(folderId, 0, nullptr, &buffer));

    return std::filesystem::path(buffer.get());
}

auto app_name() -> std::string
{
    std::string buffer;
    _get_pgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return exe.filename().replace_extension("").string();
}

auto path_portable() -> std::filesystem::path
{
    std::string buffer;
    _get_pgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return std::filesystem::canonical(exe.remove_filename());
}

auto path_portable_wide() -> std::filesystem::path
{
    std::wstring buffer;
    _get_wpgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return std::filesystem::canonical(exe.remove_filename());
}
} // namespace glow::filesystem
