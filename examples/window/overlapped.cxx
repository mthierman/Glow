#include <glow/glow.hxx>

glow::window::Overlapped window;

auto main() -> int {
    window.messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    window.min_size(200, 200);
    // window.max_size(400, 400);

    window.create();

    return glow::message::run_loop();
}
