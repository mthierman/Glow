#include <Windows.h>
#include <ShlObj.h>

#include <print>
#include <filesystem>
#include <vector>
#include <string>

#include "../filesystem/filesystem.hxx"

namespace fs = std::filesystem;

namespace glow::audio
{
auto split_clap_path(const std::wstring& in, const std::wstring& sep) -> std::vector<std::wstring>
{
    std::vector<std::wstring> res;
    std::wstringstream ss(in);
    std::wstring item;

    while (std::getline(ss, item, L';'))
        res.push_back(item);

    return res;
}

auto get_clap_search_paths() -> std::vector<std::filesystem::path>
{
    std::vector<std::filesystem::path> res;

    auto p{glow::filesystem::known_folder(FOLDERID_ProgramFilesCommon)};
    if (std::filesystem::exists(p)) res.emplace_back(p / "CLAP");

    auto q{glow::filesystem::known_folder(FOLDERID_LocalAppData)};
    if (std::filesystem::exists(q)) res.emplace_back(q / "Programs" / "Common" / "CLAP");

    std::wstring cp;
    auto len{::GetEnvironmentVariableW(L"CLAP_PATH", NULL, 0)};

    if (len > 0)
    {
        cp.resize(len);
        cp.resize(::GetEnvironmentVariableW(L"CLAP_PATH", &cp[0], len));
        auto paths{split_clap_path(cp, L";")};

        for (const auto& path : paths)
        {
            if (std::filesystem::exists(path)) res.emplace_back(path);
        }
    }

    auto paths{res};
    for (const auto& path : paths)
    {
        try
        {
            for (auto subdirectory : std::filesystem::recursive_directory_iterator(
                     path, std::filesystem::directory_options::follow_directory_symlink |
                               std::filesystem::directory_options::skip_permission_denied))
            {
                try
                {
                    if (subdirectory.is_directory()) res.emplace_back(subdirectory.path());
                }
                catch (const std::filesystem::filesystem_error& e)
                {}
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {}
    }

    return res;
}

auto list_clap_paths() -> void
{
    auto paths{get_clap_search_paths()};

    for (const auto& path : paths)
        std::println("{}", path.string());
}
} // namespace glow::audio
