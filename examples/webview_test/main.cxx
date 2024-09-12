#include <glow/glow.hxx>

namespace wm = glow::message::wm;

struct WebView final : glow::window::Window {
    WebView() {
        messages.on(WM_DESTROY, [](wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        messages.on(WM_WINDOWPOSCHANGED, [this](wm::WINDOWPOSCHANGED /* msg */) {
            webView.put_bounds(client_rect());

            return 0;
        });

        create();
        activate();

        environment.create([this]() {
            glow::log::log("Environment created!");

            webView.create(environment, hwnd.get(), [this]() {
                webView.navigate("https://mthierman.pages.dev/");

                return S_OK;
            });

            return S_OK;
        });

        // environment.close();
    }

    glow::webview::Environment environment;
    glow::webview::WebView webView;
};

auto main() -> int {
    WebView webView;

    return glow::app::run();
}
