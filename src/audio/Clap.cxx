#include <Windows.h>
#include <ShlObj.h>
#include <print>
#include <filesystem>
#include <vector>
#include <string>

#include <iostream>

namespace fs = std::filesystem;

fs::path windows_path(const KNOWNFOLDERID& id)
{
    std::wstring path;
    wchar_t* buffer;

    if (SUCCEEDED(SHGetKnownFolderPath(id, 0, nullptr, &buffer)))
    {
        fs::path data = std::wstring(buffer);
        CoTaskMemFree(buffer);

        return data;
    }

    else
        return fs::path{};
}

std::vector<std::wstring> split_clap_path(const std::wstring& in, const std::wstring& sep)
{
    std::vector<std::wstring> res;
    std::wstringstream ss(in);
    std::wstring item;

    while (std::getline(ss, item, L';'))
        res.push_back(item);

    return res;
}

std::vector<fs::path> getValidCLAPSearchPaths()
{
    std::vector<fs::path> res;

    auto p{windows_path(FOLDERID_ProgramFilesCommon)};
    if (fs::exists(p))
        res.emplace_back(p / "CLAP");

    auto q{windows_path(FOLDERID_LocalAppData)};
    if (fs::exists(q))
        res.emplace_back(q / "Programs" / "Common" / "CLAP");

    std::wstring cp;
    auto len{::GetEnvironmentVariableW(L"CLAP_PATH", NULL, 0)};

    if (len > 0)
    {
        cp.resize(len);
        cp.resize(::GetEnvironmentVariableW(L"CLAP_PATH", &cp[0], len));
        auto paths{split_clap_path(cp, L";")};

        for (const auto& path : paths)
        {
            if (fs::exists(path))
                res.emplace_back(path);
        }
    }

    auto paths{res};
    for (const auto& path : paths)
    {
        try
        {
            for (auto subdirectory : fs::recursive_directory_iterator(
                     path, fs::directory_options::follow_directory_symlink |
                               fs::directory_options::skip_permission_denied))
            {
                try
                {
                    if (subdirectory.is_directory())
                        res.emplace_back(subdirectory.path());
                }
                catch (const fs::filesystem_error& e)
                {
                }
            }
        }
        catch (const fs::filesystem_error& e)
        {
        }
    }

    return res;
}

int main()
{
    auto paths{getValidCLAPSearchPaths()};

    for (const auto& path : paths)
    {
        std::println("{}", path.string());
    }

    return 0;
}
