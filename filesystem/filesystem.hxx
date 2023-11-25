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

namespace glow::filesystem
{
auto known_folder(KNOWNFOLDERID id) -> std::filesystem::path
{
    wchar_t* buffer{nullptr};

    if (SUCCEEDED(::SHGetKnownFolderPath(id, 0, nullptr, &buffer)))
    {
        std::filesystem::path data{std::wstring(buffer)};
        ::CoTaskMemFree(buffer);

        return data;
    }

    else return {};
}

auto path_portable() -> std::filesystem::path
{
    char* pgmptr{nullptr};
    _get_pgmptr(&pgmptr);

    std::filesystem::path exe{pgmptr};

    return std::filesystem::canonical(exe.remove_filename());
}
} // namespace glow::filesystem
