#pragma once

#define GDIPVER 0x0110

#include <Windows.h>
#include <ShlObj.h>
#include <dwmapi.h>
#include <gdiplus.h>
#include <wrl.h>

#include <Unknwn.h>
#include <DispatcherQueue.h>

#include <winrt/base.h>
#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.composition.desktop.h>
#include <windows.ui.composition.interop.h>
#include <winrt/windows.ui.viewmanagement.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <print>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "winrt/Microsoft.Web.WebView2.Core.h"
#include "WebView2.h"

#include "nlohmann/json.hpp"
#include "toml++/toml.hpp"
