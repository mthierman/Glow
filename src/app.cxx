// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/app.hxx>

#include <Windows.h>

#include <stdlib.h>

namespace glow::app {
auto run() -> int {
    ::MSG msg {};
    int r {};

    while ((r = ::GetMessageW(&msg, nullptr, 0, 0)) != 0) {
        if (r == -1) {
            return EXIT_FAILURE;
        }

        else {
            ::TranslateMessage(&msg);
            ::DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}
}; // namespace glow::app
