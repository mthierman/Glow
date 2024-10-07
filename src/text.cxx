// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/text.hxx>

#include <Windows.h>

namespace glow::text {
auto convert(std::u8string_view input) -> std::optional<std::u16string> {
    auto buffer { std::string(input.begin(), input.end()) };

    if (auto length { buffer.length() }; length > 0) {
        if (length > std::numeric_limits<int>::max()) {
            return std::nullopt;
        }

        auto outputLength { ::MultiByteToWideChar(
            CP_UTF8, 0, buffer.c_str(), static_cast<int>(length), nullptr, 0) };

        std::u16string output;
        output.resize(outputLength);

        if (::MultiByteToWideChar(CP_UTF8,
                                  0,
                                  buffer.c_str(),
                                  static_cast<int>(length),
                                  reinterpret_cast<wchar_t*>(output.data()),
                                  outputLength)
            != 0) {
            return output;
        }
    }

    return std::nullopt;
}

auto convert(std::u16string_view input) -> std::optional<std::u8string> {
    auto buffer { std::wstring(input.begin(), input.end()) };

    if (auto length { buffer.length() }; length > 0) {
        if (length > std::numeric_limits<int>::max()) {
            return std::nullopt;
        }

        auto outputLength { ::WideCharToMultiByte(
            CP_UTF8, 0, buffer.c_str(), static_cast<int>(length), nullptr, 0, nullptr, nullptr) };

        std::u8string output;
        output.resize(outputLength);

        if (::WideCharToMultiByte(CP_UTF8,
                                  0,
                                  buffer.c_str(),
                                  static_cast<int>(length),
                                  reinterpret_cast<char*>(output.data()),
                                  outputLength,
                                  nullptr,
                                  nullptr)
            != 0) {
            return output;
        }
    }

    return std::nullopt;
}
}; // namespace glow::text
