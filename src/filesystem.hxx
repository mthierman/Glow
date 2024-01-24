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
#include <string>

namespace glow
{
namespace filesystem
{
auto known_folder(KNOWNFOLDERID folderId = FOLDERID_LocalAppData) -> std::filesystem::path;
auto app_name() -> std::string;
auto path_portable() -> std::filesystem::path;
auto path_portable_wide() -> std::filesystem::path;
} // namespace filesystem
} // namespace glow
