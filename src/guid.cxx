// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/guid.hxx>

#include <stdexcept>

#include <wil/win32_helpers.h>

#include <glow/log.hxx>
#include <glow/text.hxx>

namespace glow::guid {
auto create() -> ::GUID {
    ::GUID guid;

    if (auto hr { ::CoCreateGuid(&guid) }; hr != S_OK) {
        throw std::runtime_error(glow::log::format_message<std::string>(hr));
    }

    return guid;
}

auto u8string(const ::GUID& guid) -> std::u8string {
    std::wstring buffer;
    buffer.resize(wil::guid_string_buffer_length);
    ::StringFromGUID2(guid, buffer.data(), wil::guid_string_buffer_length);

    return glow::text::to_u8string(buffer);
}
}; // namespace glow::guid
