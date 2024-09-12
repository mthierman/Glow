#include <glow/glow.hxx>

namespace wm = glow::message;

struct Window final : glow::window::Window {
    Window() {
        messages.on(WM_CREATE, [this](wm::CREATE msg) {
            glow::log::log("{}", glow::text::to_string(msg.createStruct().lpszName));

            return 0;
        });

        messages.on(WM_DESTROY, [](wm::MSG /* msg */) {
            glow::system::quit();

            return 0;
        });

        messages.on(WM_KEYDOWN, [this](wm::KEYDOWN msg) {
            switch (msg.key()) {
                case VK_F11: {
                    if (!positions.fullscreen) {
                        positions.fullscreen = enable_fullscreen();
                    } else {
                        positions.fullscreen = disable_fullscreen();
                    }

                    break;
                }
            }

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
