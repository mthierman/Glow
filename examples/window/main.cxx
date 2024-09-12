#include <glow/glow.hxx>

namespace wm = glow::message::wm;

struct App final : glow::app::App {
    App() {
        messages.on(WM_NOTIFY, [](wm::NOTIFY msg) {
            glow::log::log("WM_NOTIFY message {}", msg.notification().message);
            glow::log::log("WM_NOTIFY nmhdr.code {}", msg.notification().nmhdr.code);
            glow::log::log("WM_NOTIFY code {}", std::to_underlying(msg.notification().code));

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

namespace glow::message {
enum struct Code : ::UINT { TEST, TEST2, TEST3 };
}

auto main() -> int {
    App app;

    Window window;
    window.notify(app.hwnd.get(), glow::message::Code::TEST2, "TEST");

    return glow::app::run();
}
