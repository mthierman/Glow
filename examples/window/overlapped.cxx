#include <glow/glow.hxx>

#include <iostream>

glow::window::Overlapped window;

auto WinMain([[maybe_unused]] ::HINSTANCE instance,
             [[maybe_unused]] ::HINSTANCE prevInstance,
             [[maybe_unused]] ::LPSTR cmdLine,
             [[maybe_unused]] int cmdShow) -> int {
    window.messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    window.create();

    glow::system::attach_console();

    std::cout << "TEST" << std::endl;

    return glow::message::run_loop();
}
