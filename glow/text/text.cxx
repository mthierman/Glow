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
auto narrow(std::wstring wstring) -> std::string
{
    if (!wstring.empty())
    {
        auto inSize{static_cast<int>(wstring.size())};

        auto outSize{WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                         wstring.data(), inSize, nullptr, 0, nullptr, nullptr)};

        if (outSize > 0)
        {
            std::string out;
            out.resize(static_cast<size_t>(outSize));

            if (WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                    wstring.data(), inSize, out.data(), outSize, nullptr,
                                    nullptr) > 0)
                return out;
        }
    }

    return {};
}

//==============================================================================
auto widen(std::string string) -> std::wstring
{
    if (!string.empty())
    {
        auto inSize{static_cast<int>(string.size())};

        auto outSize{
            MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, string.data(), inSize, nullptr, 0)};

        if (outSize > 0)
        {
            std::wstring out;
            out.resize(static_cast<size_t>(outSize));

            if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, string.data(), inSize,
                                    out.data(), outSize) > 0)
                return out;
        }
    }

    return {};
}

//==============================================================================
auto randomize(std::string string) -> std::string
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
