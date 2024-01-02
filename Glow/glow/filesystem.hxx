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

#include <wil/resource.h>

#include <filesystem>
#include <memory>
#include <optional>
#include <print>
#include <string>

#include <sqlite3.h>

namespace glow::filesystem
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
        auto operator()(sqlite3* pDb) noexcept -> void { sqlite3_close(pDb); }
    };
    using sqlite3_ptr = std::unique_ptr<sqlite3, sqlite3_deleter>;
    sqlite3_ptr p_db;
    std::filesystem::path path;
};

auto known_folder(const KNOWNFOLDERID& knownFolderId = FOLDERID_LocalAppData)
    -> std::optional<std::filesystem::path>;

auto program_name() -> std::string;
auto portable() -> std::filesystem::path;
auto wportable() -> std::filesystem::path;

} // namespace glow::filesystem
