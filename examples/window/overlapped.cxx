#include <glow/glow.hxx>

glow::window::Overlapped window;

auto wWinMain([[maybe_unused]] ::HINSTANCE instance,
              [[maybe_unused]] ::HINSTANCE prevInstance,
              [[maybe_unused]] wchar_t* cmdLine,
              [[maybe_unused]] int cmdShow) -> int {
    window.messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    window.create();

    return glow::message::run_loop();
}
