#pragma once

#include <Windows.h>

namespace msg
{
inline constexpr int64_t test{(WM_APP + 1)};
inline constexpr int64_t window_create{(WM_APP + 2)};
inline constexpr int64_t window_close{(WM_APP + 3)};
} // namespace msg
