#pragma once

#include <Windows.h>

#include <string>

namespace glow::filesystem
{
std::filesystem::path known_folder(const KNOWNFOLDERID& id)
{
    wchar_t* buffer{nullptr};

    if (SUCCEEDED(SHGetKnownFolderPath(id, 0, nullptr, &buffer)))
    {
        std::filesystem::path data = std::wstring(buffer);
        CoTaskMemFree(buffer);

        return data;
    }

    else return std::filesystem::path{};
}
} // namespace glow::filesystem
