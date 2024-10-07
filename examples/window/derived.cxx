#include <glow/glow.hxx>

struct Window final : glow::window::Overlapped {
    Window() {
        messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        create();
    }
};

auto WinMain([[maybe_unused]] ::HINSTANCE instance,
             [[maybe_unused]] ::HINSTANCE prevInstance,
             [[maybe_unused]] ::LPSTR cmdLine,
             [[maybe_unused]] int cmdShow) -> int {
    Window window;

    return glow::message::run_loop();
}
