#include <glow/glow.hxx>

namespace wm = glow::message;

struct Window final : glow::window::Window {
    Window() {
        messages.on(WM_CREATE, [](wm::CREATE message) {
            glow::log::log("{}", glow::text::to_string(message.createStruct().lpszName));

            return 0;
        });

        messages.on(WM_DESTROY, [](wm::DEFAULT /* message */) {
            glow::system::quit();

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
