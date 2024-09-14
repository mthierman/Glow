#include <glow/glow.hxx>

#include <WebView2.h>

namespace wm = glow::message::wm;
glow::window::WebView webView;

auto main() -> int {
    webView.messages.on(WM_DESTROY, [](wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    webView.config.backgroundColor
        = COREWEBVIEW2_COLOR { .A { 255 }, .R { 227 }, .G { 178 }, .B { 60 } };

    webView.create([]() {
        // webView.core->add_DOMContentLoaded(
        //     webView
        //         .handler<ICoreWebView2DOMContentLoadedEventHandler>(
        //             [](ICoreWebView2* sender,
        //                ICoreWebView2DOMContentLoadedEventArgs* args) -> ::HRESULT {
        //     webView.activate();

        //     return S_OK;
        // }).Get(),
        //     webView.token("DOMContentLoaded"));

        // auto handler = webView.handler<ICoreWebView2DOMContentLoadedEventHandler>(
        //     "DOMContentLoaded",
        //     [](ICoreWebView2* sender, ICoreWebView2DOMContentLoadedEventArgs* args) -> ::HRESULT
        //     { webView.activate();

        //     return S_OK;
        // });

        // auto callback =
        // wil::MakeAgileCallback<ICoreWebView2DOMContentLoadedEventHandler>(handler);

        // auto handler = webView.handler<ICoreWebView2DOMContentLoadedEventHandler>(
        //     "DOMContentLoaded",
        //     [](ICoreWebView2* sender, ICoreWebView2DOMContentLoadedEventArgs* args) -> ::HRESULT
        //     { webView.activate();

        //     return S_OK;
        // });

        auto DOMContentLoaded { webView.make_token("DOMContentLoaded") };
        webView.core->add_DOMContentLoaded(
            webView.handler<ICoreWebView2DOMContentLoadedEventHandler>(
                DOMContentLoaded.key,
                [](ICoreWebView2* sender,
                   ICoreWebView2DOMContentLoadedEventArgs* args) -> ::HRESULT {
            webView.activate();

            return S_OK;
        }),
            &DOMContentLoaded.token);

        webView.navigate("https://localhost:5173/");
    });

    return glow::app::run();
}
