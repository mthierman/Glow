// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <rpc.h>

#include <format>
#include <string>

#include <glow/text.hxx>

namespace glow::guid {
auto create() -> ::GUID;
auto u8string(const ::GUID& guid) -> std::u8string;
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

template <> struct formatter<::GUID> : formatter<string_view> {
    auto format(const ::GUID& guid, format_context& context) const noexcept {
        return formatter<string_view>::format(glow::text::to_string(glow::guid::u8string(guid)),
                                              context);
    }
};

template <> struct formatter<::GUID, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const ::GUID& guid, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(
            glow::text::to_wstring(glow::guid::u8string(guid)), context);
    }
};
} // namespace std
