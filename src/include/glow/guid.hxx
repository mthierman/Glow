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

#include <glow/math.hxx>

namespace glow::guid {
auto create() -> ::GUID;
auto to_string(const ::GUID& guid) -> std::string;
auto to_wstring(const ::GUID& guid) -> std::wstring;
}; // namespace glow::guid

namespace std {
template <> struct hash<::GUID> {
    auto operator()(const ::GUID& guid) const noexcept {
        ::RPC_STATUS status { RPC_S_OK };
        return glow::math::check_safe_size<size_t>(::UuidHash(&const_cast<::GUID&>(guid), &status));
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
        return formatter<string_view>::format(glow::guid::to_string(guid), context);
    }
};

template <> struct formatter<::GUID, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const ::GUID& guid, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(glow::guid::to_wstring(guid), context);
    }
};
} // namespace std
