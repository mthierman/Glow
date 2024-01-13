// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "text.hxx"

namespace glow::text
{
auto narrow(std::wstring utf16) -> std::string
{
    constexpr int max{std::numeric_limits<int>::max()};

    if (utf16.length() > static_cast<size_t>(max))
        throw std::overflow_error("Input string too long");

    auto length{WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                    utf16.data(), static_cast<int>(utf16.length()), nullptr, 0,
                                    nullptr, nullptr)};

    std::string utf8(length, 0);

    if (length == 0) throw std::runtime_error(glow::console::last_error_string());

    else if (WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS, utf16.data(),
                                 static_cast<int>(utf16.length()), utf8.data(), length, nullptr,
                                 nullptr) > 0)
        return utf8;

    else throw std::runtime_error(glow::console::last_error_string());
}

auto widen(std::string utf8) -> std::wstring
{
    constexpr int max{std::numeric_limits<int>::max()};

    if (utf8.length() > static_cast<size_t>(max))
        throw std::overflow_error("Input string too long");

    auto length{MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(),
                                    static_cast<int>(utf8.length()), nullptr, 0)};

    std::wstring utf16(length, 0);

    if (length == 0) throw std::runtime_error(glow::console::last_error_string());

    else if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(),
                                 static_cast<int>(utf8.length()), utf16.data(), length) > 0)
        return utf16;

    else throw std::runtime_error(glow::console::last_error_string());
}

auto randomize(std::string string) -> std::string
{
    return string.append(std::to_string(random<int64_t>()));
}

auto guid() -> GUID
{
    GUID guid;

    if (FAILED(CoCreateGuid(&guid))) throw std::runtime_error("GUID creation failure");

    else return guid;
}

auto guid_string(GUID guid) -> std::string
{
    std::wstring buffer(wil::guid_string_buffer_length, 0);

    auto length{StringFromGUID2(guid, buffer.data(), wil::guid_string_buffer_length)};

    if (length == 0) throw std::runtime_error("String from GUID failure");

    else return narrow(buffer);
}
} // namespace glow::text
