// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <string>
#include <json.hpp>

enum struct CODE : unsigned int;

namespace glow
{
struct Notification
{
    ::NMHDR nmhdr{};

    ::HWND hwnd{};
    size_t id{};
    CODE code{};
    std::string message{};

    auto reset() -> void;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Notification, message)
};
} // namespace glow
