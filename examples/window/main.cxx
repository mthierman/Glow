#include <glow/glow.hxx>

struct Window final : glow::window::Window {
    Window() {
        messages.on(WM_CREATE, [](glow::message::CREATE message) {
            glow::log::log("{}", glow::text::to_string(message.createStruct().lpszName));

            return 0;
        });

        messages.on(WM_DESTROY, [](glow::message::Message /* message */) {
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
