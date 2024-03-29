// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "guid.hxx"
#include "text.hxx"
#include <wil/win32_helpers.h>

namespace glow
{
GUID::GUID()
{
    ::CoCreateGuid(&guid);

    std::wstring buffer(wil::guid_string_buffer_length, 0);
    ::StringFromGUID2(guid, buffer.data(), wil::guid_string_buffer_length);

    string.assign(glow::lowercase(glow::string(buffer)));
}
} // namespace glow
