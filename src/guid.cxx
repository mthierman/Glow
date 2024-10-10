// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/guid.hxx>

#include <wil/win32_helpers.h>

#include <glow/system.hxx>
#include <glow/text.hxx>

namespace glow::guid {
auto create() -> std::expected<::GUID, std::u8string> {
    ::GUID guid;

    if (auto errorCode { ::CoCreateGuid(&guid) }; errorCode == S_OK) {
        return guid;
    } else {
        return std::unexpected(glow::system::format_message(errorCode));
    }
}

auto u8string(const ::GUID& guid) -> std::optional<std::u8string> {
    std::wstring buffer;
    buffer.resize(wil::guid_string_buffer_length);

    if (::StringFromGUID2(guid, buffer.data(), wil::guid_string_buffer_length) == 0) {
        return std::nullopt;
    }

    if (auto u8buffer { glow::text::u8string(buffer) }) {
        return *u8buffer;
    } else {
        return std::nullopt;
    }
}

auto u16string(const ::GUID& guid) -> std::optional<std::u16string> {
    std::wstring buffer;
    buffer.resize(wil::guid_string_buffer_length);

    if (::StringFromGUID2(guid, buffer.data(), wil::guid_string_buffer_length) == 0) {
        return std::nullopt;
    }

    return reinterpret_cast<char16_t*>(buffer.data());
}
}; // namespace glow::guid
