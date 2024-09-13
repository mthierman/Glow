#include <glow/glow.hxx>

namespace wm = glow::message::wm;

struct WebView final : glow::window::WebView {
    WebView() {
        messages.on(WM_DESTROY, [](wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        create([this]() { navigate("https://mthierman.pages.dev/"); });
    }
};

auto main() -> int {
    WebView webView;

    return glow::app::run();
}
