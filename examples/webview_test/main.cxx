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

        environment.create([this]() {
            glow::log::log("Environment created!");

            webView.create(environment, hwnd.get());
            // webView.create(environment, hwnd.get(), []() { return S_OK; });

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
