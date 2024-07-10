#include <glow/messages.hxx>
#include <glow/window.hxx>

struct Window final : glow::window::Window {
    Window() {
        message(WM_CREATE, [](glow::messages::wm_create message) {
            glow::system::dbg("WM_CREATE");
            return 0;
        });

        message(WM_DESTROY, [](glow::messages::wm message) {
            glow::system::quit();
            return 0;
        });

        message(WM_KEYDOWN, [this](glow::messages::wm_keydown_keyup message) {
            switch (auto key { message.key() }; key) {
                case 'N': {
                    glow::window::toggle_fullscreen(m_hwnd.get());
                    break;
                }
            }
            return 0;
        });

        create("Testing");

        glow::window::enable_dark_mode(m_hwnd.get());
        glow::window::set_backdrop(m_hwnd.get(), DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);
    }
};

auto main() -> int {
    Window window;
    return glow::messages::run_loop();
}
