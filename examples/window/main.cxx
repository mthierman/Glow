#include <glow/glow.hxx>

namespace wm = glow::message;

struct App final : glow::app::App {
    App() {
        messages.on(WM_NOTIFY, [](wm::MSG /* msg */) {
            glow::log::log("WM_NOTIFY");

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

    window.messages.send(app.hwnd.get(), WM_NOTIFY, 0, 0);
    window.messages.send(app.hwnd.get(), WM_NOTIFY, 0, 0);

    if (app.hwnd) {
        glow::log::log("app.hwnd exists");
    }

    return glow::app::run();
}
