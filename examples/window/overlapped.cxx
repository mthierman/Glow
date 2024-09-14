#include <glow/glow.hxx>

glow::window::Overlapped window;

auto main() -> int {
    window.messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    window.create();

    window.set_title("TEST");

    // window.background_style(glow::window::Background::Style::Transparent);

    // window.background_style(glow::window::Background::Style::System);
    // window.background_dark(glow::color::Color(50, 50, 50));
    // window.background_light(glow::color::Color(200, 200, 200));

    window.background_style(glow::window::Background::Style::Custom);
    // window.background_custom(glow::color::Color(0, 0, 0));
    window.background_custom(glow::color::Color(255, 255, 255));

    return glow::app::run();
}
