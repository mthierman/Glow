// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/guid.hxx>

#include <objbase.h>

#include <stdexcept>

#include <wil/result.h>
#include <wil/win32_helpers.h>

#include <glow/text.hxx>

namespace glow::guid {
auto create() -> ::GUID {
    ::GUID guid;
    THROW_IF_FAILED(::CoCreateGuid(&guid));

    return guid;
}

auto to_wstring(const ::GUID& guid) -> std::wstring {
    std::wstring buffer;
    buffer.resize(wil::guid_string_buffer_length);

    if (::StringFromGUID2(guid, buffer.data(), wil::guid_string_buffer_length) == 0) {
        throw std::overflow_error("Buffer is too small to contain the string");
    }

    return buffer;
}

auto to_string(const ::GUID& guid) -> std::string {
    return glow::text::utf16_to_utf8(to_wstring(guid));
}
}; // namespace glow::guid
