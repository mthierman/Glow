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
auto string(std::wstring wstring) -> std::string
{
    if (wstring.empty()) return {};

    auto safeSize{safe_size<size_t, int>(wstring.length())};

    auto length{::WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                      wstring.data(), safeSize, nullptr, 0, nullptr, nullptr)};

    std::string utf8(length, 0);

    if (::WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS, wstring.data(),
                              safeSize, utf8.data(), length, nullptr, nullptr) > 0)
        return utf8;

    else throw std::runtime_error(glow::last_error_string());
}

auto wstring(std::string string) -> std::wstring
{
    if (string.empty()) return {};

    auto safeSize{safe_size<size_t, int>(string.length())};

    auto length{
        ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, string.data(), safeSize, nullptr, 0)};

    std::wstring utf16(length, 0);

    if (::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, string.data(), safeSize, utf16.data(),
                              length) > 0)
        return utf16;

    else throw std::runtime_error(glow::last_error_string());
}

auto trim(std::string string) -> std::string
{
    auto is_space{[](unsigned char c) { return std::isspace(c); }};

    auto trimmed{string | std::views::drop_while(is_space) | std::views::reverse |
                 std::views::drop_while(is_space) | std::views::reverse};

    return {trimmed.begin(), trimmed.end()};
}
} // namespace glow
