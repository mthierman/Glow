// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "filesystem.hxx"

namespace glow
{
namespace filesystem
{
auto known_folder(KNOWNFOLDERID folderId) -> std::filesystem::path
{
    wil::unique_cotaskmem_string buffer{};
    SHGetKnownFolderPath(folderId, 0, nullptr, &buffer);

    return std::filesystem::path(buffer.get());
}

auto app_name() -> std::string
{
    std::string buffer{};
    _get_pgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return exe.filename().replace_extension("").string();
}

auto path_portable() -> std::filesystem::path
{
    std::string buffer{};
    _get_pgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return std::filesystem::canonical(exe.remove_filename());
}

auto path_portable_wide() -> std::filesystem::path
{
    std::wstring buffer{};
    _get_wpgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return std::filesystem::canonical(exe.remove_filename());
}
} // namespace filesystem
} // namespace glow
