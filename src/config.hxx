// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <string>

namespace glow
{
namespace config
{
inline std::string version{"@PROJECT_VERSION@"};
inline std::string webview2{"@WEBVIEW2@"};
inline std::string webview2_version{"@WEBVIEW2_VERSION@"};
inline std::string cppwinrt{"@CPPWINRT@"};
inline std::string cppwinrt_version{"@CPPWINRT_VERSION@"};
inline std::string wil{"@WIL@"};
inline std::string wil_version{"@WIL_VERSION@"};
} // namespace config
} // namespace glow
