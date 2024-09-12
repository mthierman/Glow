#include <glow/glow.hxx>

namespace wm = glow::message::wm;

struct WebView final : glow::window::Window {
    WebView() {
        messages.on(WM_DESTROY, [](wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        create();
        activate();

        environment.create([]() {
            glow::log::log("Environment created!");

            return S_OK;
        });

        environment.close();
    }

    glow::webview::Environment environment;
};

auto main() -> int {
    WebView webView;

    return glow::app::run();
}
