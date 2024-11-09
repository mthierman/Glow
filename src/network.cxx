// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/network.hxx>

#include <Windows.h>

#include <ShlObj.h>
#include <urlmon.h>

#include <wil/win32_helpers.h>

#include <glow/filesystem.hxx>
#include <glow/text.hxx>

namespace glow::network {
auto download_file(std::string_view url, const std::filesystem::path& path) -> bool {
    if (auto converted { glow::text::u16string(url) }) {
        if (auto result { ::URLDownloadToFileW(
                nullptr, glow::text::c_str(*converted), path.c_str(), 0, nullptr) };
            result == S_OK) {
            return true;
        } else {
            return true;
        }
    }

    return false;
}
}; // namespace glow::network
