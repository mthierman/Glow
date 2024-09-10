#include <print>
#include <stdlib.h>

#include <glow/filesystem.hxx>
#include <glow/log.hxx>

auto main() -> int {
    // auto knownFolder { glow::filesystem::known_folder(FOLDERID_Documents, { "test", "test" }) };

    glow::log::log("{}", glow::text::utf16_to_utf8(glow::filesystem::temp_folder().wstring()));

    return EXIT_SUCCESS;
}
