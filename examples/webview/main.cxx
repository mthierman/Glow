#include <glow/glow.hxx>

namespace wm = glow::message::wm;

struct WebViewTest final : glow::window::Window {
    WebViewTest() {
        messages.on(WM_DESTROY, [](wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        create();
        activate();
        create_webview();
    }
};

auto main() -> int {
    winrt::init_apartment(winrt::apartment_type::single_threaded);

    WebViewTest webViewTest;

    return glow::app::run();
}
