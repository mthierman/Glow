// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "text.hxx"

namespace glow
{
namespace text
{
auto to_utf8(std::wstring utf16) -> std::string
{
    if (utf16.empty()) return {};

    auto safeSize{safe_size<size_t, int>(utf16.length())};

    auto length{WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                    utf16.data(), safeSize, nullptr, 0, nullptr, nullptr)};

    std::string utf8(length, 0);

    if (WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS, utf16.data(),
                            safeSize, utf8.data(), length, nullptr, nullptr) > 0)
        return utf8;

    else throw std::runtime_error(glow::console::last_error_string());
}

auto to_utf16(std::string utf8) -> std::wstring
{
    if (utf8.empty()) return {};

    auto safeSize{safe_size<size_t, int>(utf8.length())};

    auto length{
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), safeSize, nullptr, 0)};

    std::wstring utf16(length, 0);

    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), safeSize, utf16.data(),
                            length) > 0)
        return utf16;

    else throw std::runtime_error(glow::console::last_error_string());
}

auto append_random(std::string string) -> std::string
{
    return string.append(std::to_string(make_random<int>()));
}
} // namespace text
} // namespace glow
