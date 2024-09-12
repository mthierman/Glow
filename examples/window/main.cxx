#include <glow/glow.hxx>

enum struct glow::message::Code : ::UINT { HELLO, GOODBYE };
namespace wm = glow::message::wm;

struct App final : glow::app::App {
    App() {
        messages.on(WM_NOTIFY, [](wm::NOTIFY msg) {
            switch (msg.notification().code) {
                case glow::message::Code::HELLO: {
                    glow::log::log("HELLO: {}", msg.notification().message);
                } break;
                case glow::message::Code::GOODBYE: {
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

    window.notify_app(glow::message::Code::HELLO, "TEST1");
    window.notify_app(glow::message::Code::GOODBYE, "TEST2");

    return glow::app::run();
}
