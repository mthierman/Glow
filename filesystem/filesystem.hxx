#pragma once

#include <Windows.h>

#include <filesystem>
#include <string>

namespace glow::filesystem
{
auto known_folder(const KNOWNFOLDERID& id) -> std::filesystem::path
{
    wchar_t* buffer{nullptr};

    if (SUCCEEDED(SHGetKnownFolderPath(id, 0, nullptr, &buffer)))
    {
        std::filesystem::path data{std::wstring(buffer)};
        CoTaskMemFree(buffer);

        return data;
    }

    else return {};
}

auto path_portable() -> std::filesystem::path
{
    wchar_t* wpgmptr{nullptr};
    _get_wpgmptr(&wpgmptr);

    std::filesystem::path exe{wpgmptr};

    return std::filesystem::canonical(exe.remove_filename());
}
} // namespace glow::filesystem
