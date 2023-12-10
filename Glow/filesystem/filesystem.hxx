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

#include <filesystem>
#include <string>
#include <vector>

//==============================================================================
namespace glow::filesystem
{

//==============================================================================
auto known_folder(KNOWNFOLDERID knownFolderId) -> std::filesystem::path;
auto get_pgmptr() -> std::filesystem::path;
auto get_wpgmptr() -> std::filesystem::path;

//==============================================================================
} // namespace glow::filesystem
