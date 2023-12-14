// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <text/text.hxx>

//==============================================================================
namespace glow::text
{

//==============================================================================
auto narrow(const std::wstring& utf16) -> std::string
{
    constexpr int intMax = std::numeric_limits<int>::max();
    if (utf16.length() > static_cast<size_t>(intMax))
        throw std::overflow_error("Input string is too long: size_t doesn't fit into int");

    std::string utf8;

    auto length{WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                    utf16.data(), static_cast<int>(utf16.length()), nullptr, 0,
                                    nullptr, nullptr)};

    if (length > 0)
    {
        if (WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS, utf16.data(),
                                static_cast<int>(utf16.length()), utf8.data(), length, nullptr,
                                nullptr) > 0)
            return utf8;

        else return {};
    }

    else return {};
}

//==============================================================================
auto widen(const std::string& utf8) -> std::wstring
{
    constexpr int intMax = std::numeric_limits<int>::max();
    if (utf8.length() > static_cast<size_t>(intMax))
        throw std::overflow_error("Input string is too long: size_t doesn't fit into int");

    std::wstring utf16;

    auto length{MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(),
                                    static_cast<int>(utf8.length()), nullptr, 0)};

    if (length > 0)
    {
        if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(),
                                static_cast<int>(utf8.length()), utf16.data(), length) > 0)
            return utf16;

        else return {};
    }

    else return {};
}

//==============================================================================
auto randomize(const std::string& string) -> std::string
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(1.0, 10.0);
    auto randomDouble{dist(mt)};
    auto randomNumber{std::to_string(randomDouble)};
    std::erase(randomNumber, '.');

    return (string + randomNumber);
}

//==============================================================================
} // namespace glow::text
