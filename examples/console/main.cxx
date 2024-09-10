#include <stdlib.h>

#include <glow/log.hxx>
#include <glow/system.hxx>
#include <glow/text.hxx>

auto main() -> int {
    auto guid { glow::system::make_guid() };

    glow::log::log("{}", glow::text::guid_to_string(guid));
    glow::log::log(L"{}", glow::text::guid_to_wstring(guid));

    return EXIT_SUCCESS;
}
