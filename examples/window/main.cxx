#include <glow/glow.hxx>

struct Window final : glow::window::Window {
    Window() {
        messages.on(WM_CREATE, [](glow::message::CREATE message) {
            //
            auto createStruct { message.createStruct() };
            glow::log::log("{}", glow::text::to_string(createStruct.lpszName));

            return 0;
        });

        // message(WM_CREATE, [](glow::message::wm_create /* message */) {
        //     glow::log::log("WM_CREATE");
        //     return 0;
        // });

        // message(WM_DESTROY, [](glow::message::wm /* message */) {
        //     glow::system::quit();
        //     return 0;
        // });

        // message(WM_KEYDOWN, [this](glow::message::wm_keydown_keyup message) {
        //     switch (auto key { message.key() }; key) {
        //         case 'N': {
        //             glow::window::toggle_fullscreen(m_hwnd.get());
        //             break;
        //         }
        //     }
        //     return 0;
        // });

        create();
        activate();

        // glow::window::enable_dark_mode(m_hwnd.get());
        // glow::window::set_backdrop(m_hwnd.get(), DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);
    }
};

auto main() -> int {
    Window window;

    return glow::app::run();
}
