// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <rpc.h>

#include <expected>
#include <format>
#include <string>

#include <glow/text.hxx>

namespace glow::guid {
auto create() -> std::expected<::GUID, std::u8string>;
auto u8string(const ::GUID& guid) -> std::optional<std::u8string>;
auto u16string(const ::GUID& guid) -> std::optional<std::u16string>;
}; // namespace glow::guid

namespace std {
template <> struct hash<::GUID> {
    auto operator()(const ::GUID& guid) const noexcept {
        ::RPC_STATUS status { RPC_S_OK };
        return static_cast<size_t>(::UuidHash(&const_cast<::GUID&>(guid), &status));
    }
};

template <> struct less<::GUID> {
    auto operator()(const ::GUID& lhs, const ::GUID& rhs) const noexcept {
        ::RPC_STATUS status { RPC_S_OK };
        return ::UuidCompare(&const_cast<::GUID&>(lhs), &const_cast<::GUID&>(rhs), &status) == -1
            ? true
            : false;
    }
};
} // namespace std

namespace std {
template <> struct formatter<::GUID> : formatter<string_view> {
    auto format(const ::GUID& guid, format_context& context) const noexcept {
        if (auto converted { glow::guid::u8string(guid) }) {
            return formatter<string_view>::format(glow::text::c_str(*converted), context);
        } else {
            return formatter<string_view>::format({}, context);
        }
    }
};

template <> struct formatter<::GUID, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const ::GUID& guid, wformat_context& context) const noexcept {
        if (auto converted { glow::guid::u16string(guid) }) {
            return formatter<wstring_view, wchar_t>::format(glow::text::c_str(*converted), context);
        } else {
            return formatter<wstring_view, wchar_t>::format({}, context);
        }
    }
};
} // namespace std
