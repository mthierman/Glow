#include <glow/glow.hxx>

namespace wm = glow::message::wm;

glow::window::WebView webView;

auto main() -> int {
    webView.messages.on(WM_DESTROY, [](wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    webView.create([]() { webView.navigate("https://localhost:5173/"); });

    return glow::app::run();
}
