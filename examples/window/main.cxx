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
                    if (!states.fullscreen) {
                        states.fullscreen = enable_fullscreen();
                    } else {
                        states.fullscreen = disable_fullscreen();
                    }
                } break;

                case VK_F4: {
                    if (states.centered) {
                        set_position(positions.restoreCentered);
                        states.centered = false;
                    } else {
                        positions.restoreCentered = positions.window;
                        center();
                        states.centered = true;
                    }
                } break;
            }

            return 0;
        });

        // set_background_type(glow::window::BackgroundType::BG_SYSTEM);
        set_background_color(0, 0, 255);

        create();
        activate();

        // set_background_type(glow::window::BackgroundType::BG_CUSTOM);
    }
};

auto main() -> int {
    App app;

    Window window;

    window.notify_app(glow::message::Code::HELLO, "TEST1");
    window.notify_app(glow::message::Code::GOODBYE, "TEST2");

    return glow::app::run();
}
