#include <print>
#include <stdlib.h>

#include <glow/filesystem.hxx>
#include <glow/log.hxx>

auto main() -> int {
    auto knownFolder { glow::filesystem::known_folder(FOLDERID_Documents, { "test", "test" }) };

    // glow::log::log("{}", glow::text::utf16_to_utf8(glow::filesystem::temp_folder().wstring()));
    // std::vformat(L"", knownFolder.wstring());
    // glow::log::println("{}", knownFolder);

    glow::log::log(glow::filesystem::to_string(knownFolder));

    return EXIT_SUCCESS;
}
