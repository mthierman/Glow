// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/guid.hxx>

#include <wil/win32_helpers.h>

#include <glow/log.hxx>
#include <glow/text.hxx>

namespace glow::guid {
auto create() -> ::GUID {
    ::GUID guid;
    ::CoCreateGuid(&guid);

    return guid;
}

auto u8string(const ::GUID& guid) -> std::u8string {
    std::wstring buffer;
    buffer.resize(wil::guid_string_buffer_length);

    if (::StringFromGUID2(guid, buffer.data(), wil::guid_string_buffer_length) != 0) {
        if (auto stringified { glow::text::u8string(buffer) }; stringified) {
            return stringified.value();
        }
    }

    return {};
}

auto u16string(const ::GUID& guid) -> std::u16string {
    std::wstring buffer;
    buffer.resize(wil::guid_string_buffer_length);

    if (::StringFromGUID2(guid, buffer.data(), wil::guid_string_buffer_length) != 0) {
        return reinterpret_cast<char16_t*>(buffer.data());
    }

    return {};
}
}; // namespace glow::guid
