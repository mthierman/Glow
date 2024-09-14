#include <glow/glow.hxx>

glow::window::Overlapped window;

auto main() -> int {
    window.messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    window.create();
    window.background_style(glow::window::BackgroundStyle::Custom);
    window.background_custom(glow::color::Color(255, 0, 0));

    return glow::app::run();
}
