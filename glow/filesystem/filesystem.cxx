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

auto known_folder(const KNOWNFOLDERID& knownFolderId) -> std::filesystem::path
{
    wil::unique_cotaskmem_string buffer;
    winrt::check_hresult(SHGetKnownFolderPath(knownFolderId, 0, nullptr, &buffer));

    return std::filesystem::path(buffer.get());
}

auto get_pgmptr() -> std::filesystem::path
{
    std::string buffer;
    _get_pgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return std::filesystem::canonical(exe.remove_filename());
}

auto get_wpgmptr() -> std::filesystem::path
{
    std::wstring buffer;
    _get_wpgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return std::filesystem::canonical(exe.remove_filename());
}

} // namespace glow::filesystem
