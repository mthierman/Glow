#pragma once

#include <Windows.h>
#include <cstdint>

namespace msg
{
inline constexpr uint64_t test{(WM_APP + 1)};
inline constexpr uint64_t window_create{(WM_APP + 2)};
inline constexpr uint64_t window_close{(WM_APP + 3)};
} // namespace msg
