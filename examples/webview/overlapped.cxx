#include <glow/glow.hxx>

namespace wm = glow::message::wm;

glow::window::WebView webView;

auto main() -> int {
    webView.messages.on(WM_DESTROY, [](wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    webView.set_background_color({ 227, 178, 60 });
    webView.set_background_style(glow::window::BackgroundStyle::Custom);

    // webView.create();

    // webView.create([]() {
    //     webView.core->add_DOMContentLoaded(
    //         webView
    //             .handler<ICoreWebView2DOMContentLoadedEventHandler>(
    //                 [](ICoreWebView2* /* sender */,
    //                    ICoreWebView2DOMContentLoadedEventArgs* /* args */) -> ::HRESULT {
    //         glow::log::log("DOMContentLoaded");
    //         webView.core->remove_DOMContentLoaded(webView.tokens("DOMContentLoaded"));

    //         return S_OK;
    //     }).Get(),
    //         &webView.tokens("DOMContentLoaded"));

    webView.create([]() {
        webView.core->add_DOMContentLoaded(
            webView.handler<ICoreWebView2DOMContentLoadedEventHandler>(
                [](ICoreWebView2* /* sender */,
                   ICoreWebView2DOMContentLoadedEventArgs* /* args */) -> ::HRESULT {
            glow::log::log("DOMContentLoaded");
            webView.core->remove_DOMContentLoaded(webView.token("DOMContentLoaded"));

            return S_OK;
        }),
            &webView.token("DOMContentLoaded"));

        webView.navigate("https://localhost:5173/");
        // webView.navigate("https://google.com/");
    }, true);

    return glow::app::run();
}
