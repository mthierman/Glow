#include <glow/glow.hxx>

glow::window::WebView webView;

auto main() -> int {
    webView.messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    webView.create([]() { webView.navigate("http://localhost:5173/"); });

    return glow::app::run();
}
