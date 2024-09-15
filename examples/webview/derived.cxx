#include <glow/glow.hxx>

struct WebView final : glow::window::WebView {
    WebView() {
        messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        create([this]() { navigate("http://localhost:5173/"); });
    }
};

auto main() -> int {
    WebView webView;

    return glow::app::run();
}
