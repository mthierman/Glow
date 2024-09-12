#include <glow/glow.hxx>

struct Window final : glow::window::Window {
    Window() {
        messages.on(WM_DESTROY, [](wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        create();
        activate();
    }
};

auto main() -> int {
    Window window;

    return glow::app::run();
}
