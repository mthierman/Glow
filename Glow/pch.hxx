// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#define GDIPVER 0x0110

#include <Windows.h>
#include <comdef.h>
#include <dwmapi.h>
#include <gdiplus.h>
#include <shellapi.h>
#include <ShlObj.h>
#include <Unknwn.h>

#include <wil/com.h>
#include <wil/resource.h>
#include <wrl.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <algorithm>
#include <bit>
#include <filesystem>
#include <iostream>
#include <limits>
#include <memory>
#include <print>
#include <random>
#include <source_location>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

#include <sqlite3.h>
#include <nlohmann/json.hpp>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
