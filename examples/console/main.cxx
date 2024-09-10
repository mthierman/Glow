#include <stdlib.h>

#include <glow/log.hxx>
#include <glow/system.hxx>
#include <glow/text.hxx>

auto main() -> int {
    auto guid { glow::guid::create() };

    glow::log::log("{}", glow::guid::to_string(guid));
    glow::log::log(L"{}", glow::guid::to_wstring(guid));

    return EXIT_SUCCESS;
}
