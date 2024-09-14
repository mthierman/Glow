#include <glow/glow.hxx>

#include <WebView2.h>

namespace wm = glow::message::wm;

glow::window::WebView webView;
std::unordered_map<std::string, ::EventRegistrationToken> tokens;

auto main() -> int {
    webView.messages.on(WM_DESTROY, [](wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    webView.config.backgroundColor
        = COREWEBVIEW2_COLOR { .A { 255 }, .R { 227 }, .G { 178 }, .B { 60 } };

    webView.create([]() {
        webView.core->add_DOMContentLoaded(
            webView
                .handler<ICoreWebView2DOMContentLoadedEventHandler>(
                    [](ICoreWebView2* sender,
                       ICoreWebView2DOMContentLoadedEventArgs* args) -> ::HRESULT {
            webView.activate();
            webView.core->remove_DOMContentLoaded(webView.tokens("DOMContentLoaded"));

            return S_OK;
        }).Get(),
            &webView.tokens("DOMContentLoaded"));

        webView.navigate("https://localhost:5173/");
    });

    return glow::app::run();
}
