#include <Windows.h>

#include <print>

#include <filesystem/filesystem.hxx>
#include <text/text.hxx>

using std::println;

auto main() -> int
{
    std::println("{}", glow::filesystem::known_folder(FOLDERID_LocalAppData).string());
    std::println("{}", glow::filesystem::path_portable().string());
    std::println("{}", glow::text::randomize("Randomized!"));

    println("{}", std::string(WEBVIEW));
    println("{}", std::string(CPPWINRT_VERSION));

    return 0;
}
