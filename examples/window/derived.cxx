#include <glow/glow.hxx>

struct Window final : glow::window::Overlapped {
    Window() {
        messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        create();
    }
};

auto main() -> int {
    Window window;

    return glow::message::run_loop();
}
