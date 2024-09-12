#include <glow/glow.hxx>

namespace wm = glow::message::wm;

struct WebView final : glow::window::Window {
    WebView() {
        messages.on(WM_DESTROY, [](wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        messages.on(WM_WINDOWPOSCHANGED, [this](wm::WINDOWPOSCHANGED /* msg */) {
            controller.put_bounds(client_rect());

            return 0;
        });

        create();
        activate();

        environment.create([this]() {
            controller.create(environment, hwnd.get(), [this]() {
                controller.navigate("https://mthierman.pages.dev/");
            });
        });
    }

    glow::webview::Environment environment;
    glow::webview::Controller controller;
};

auto main() -> int {
    WebView webView;

    return glow::app::run();
}
