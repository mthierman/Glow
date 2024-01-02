// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <string>

namespace glow::config
{

std::string name{"@PROJECT_NAME@"};
std::string version{"@PROJECT_VERSION@"};
std::string webView2{"@WEBVIEW2@"};
std::string webView2Version{"@WEBVIEW2_VER@"};
std::string cppwinrt{"@WEBVIEW2@"};
std::string cppwinrtVersion{"@WEBVIEW2_VER@"};
std::string wil{"@WEBVIEW2@"};
std::string wilVersion{"@WEBVIEW2_VER@"};

} // namespace glow::config
