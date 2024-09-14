#include <glow/glow.hxx>

namespace wm = glow::message::wm;

glow::window::Overlapped window;

auto main() -> int {
    window.messages.on(WM_DESTROY, [](wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    window.create();
    window.activate();

    return glow::app::run();
}
