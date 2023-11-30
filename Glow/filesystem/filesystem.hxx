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
#include <ShlObj.h>

#include <filesystem>
#include <string>
#include <vector>

namespace glow::filesystem
{
auto known_folder(KNOWNFOLDERID knownFolderId) -> std::filesystem::path
{
    std::wstring wstring;
    auto buffer{wstring.data()};

    if (SUCCEEDED(::SHGetKnownFolderPath(knownFolderId, 0, nullptr, &buffer)))
        return std::filesystem::path(buffer);

    else return {};
}

auto get_pgmptr() -> std::filesystem::path
{
    std::string string;
    auto buffer{string.data()};

    _get_pgmptr(&buffer);

    std::filesystem::path exe{buffer};

    return std::filesystem::canonical(exe.remove_filename());
}

auto get_wpgmptr() -> std::filesystem::path
{
    std::wstring wstring;
    auto buffer{wstring.data()};

    _get_wpgmptr(&buffer);

    std::filesystem::path exe{buffer};

    return std::filesystem::canonical(exe.remove_filename());
}
} // namespace glow::filesystem
