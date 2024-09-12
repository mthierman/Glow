#include <glow/glow.hxx>

namespace wm = glow::message::wm;

// namespace glow::message {
// enum struct Code : ::UINT { HELLO, GOODBYE };
// }

enum struct glow::message::Code : ::UINT { HELLO, GOODBYE };
struct App final : glow::app::App {
    App() {
        messages.on(WM_NOTIFY, [](wm::NOTIFY msg) {
            using enum glow::message::Code;

            switch (msg.notification().code) {
                case HELLO: {
                    glow::log::log("HELLO: {}", msg.notification().message);
                } break;
                case GOODBYE: {
                    glow::log::log("GOODBYE: {}", msg.notification().message);
                } break;
            }

            return 0;
        });

        create();
    }
};

struct Window final : glow::window::Window {
    Window() {
        messages.on(WM_DESTROY, [](wm::DESTROY msg) {
            msg.quit();

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
    App app;

    Window window;

    window.notify(glow::message::Code::HELLO, "TEST1");
    window.notify(glow::message::Code::GOODBYE, "TEST2");

    return glow::app::run();
}
