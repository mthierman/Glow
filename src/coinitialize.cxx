// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "coinitialize.hxx"

namespace glow
{
CoInitialize::CoInitialize() : result{::CoInitializeEx(nullptr, ::COINIT_APARTMENTTHREADED)}
{
    if (FAILED(result)) { throw std::runtime_error("CoInitialize failure"); }
}

CoInitialize::~CoInitialize()
{
    if (SUCCEEDED(result)) CoUninitialize();
}

CoInitialize::operator ::HRESULT() const { return result; }
} // namespace glow
