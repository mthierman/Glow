#include <glow/glow.hxx>

namespace wm = glow::message::wm;

glow::window::Overlapped window;

auto main() -> int {
    window.messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    window.create();

    return glow::app::run();
}
