#include <glow/messages.hxx>
#include <glow/webviewrt.hxx>
#include <glow/window.hxx>

struct Window : glow::window::Window {
    Window() {
        message(WM_CREATE, [this](glow::messages::wm_create message) {
            glow::system::dbg("WM_CREATE");
            m_webViewEnvironment.create(m_hwnd.get());
            return 0;
        });

        message(WM_DESTROY, [](glow::messages::wm message) {
            glow::system::quit();
            return 0;
        });

        create();
    }

    glow::webviewrt::WebViewEnvironment m_webViewEnvironment;
};

auto main() -> int {
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    Window window;
    return glow::messages::run_loop();
}