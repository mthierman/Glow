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
        webView.navigate("https://localhost:5173/");

        // ::EventRegistrationToken token;
        // webView.core->add_NavigationCompleted(
        //     wil::MakeAgileCallback<ICoreWebView2NavigationCompletedEventHandler>(
        //         [](ICoreWebView2* sender,
        //            ICoreWebView2NavigationCompletedEventArgs* args) -> ::HRESULT {
        //     webView.activate();

        //     return S_OK;
        // }).Get(),
        //     &token);

        ::EventRegistrationToken token1;
        webView.core->add_DOMContentLoaded(
            wil::MakeAgileCallback<ICoreWebView2DOMContentLoadedEventHandler>(
                [](ICoreWebView2* sender,
                   ICoreWebView2DOMContentLoadedEventArgs* args) -> ::HRESULT {
            webView.activate();

            return S_OK;
        }).Get(),
            &token1);

        auto lambda
            = [](ICoreWebView2* sender, ICoreWebView2DOMContentLoadedEventArgs* args) -> ::HRESULT {
            glow::log::log("TEST");
            return S_OK;
        };

        // std::function<::HRESULT(ICoreWebView2*, ICoreWebView2DOMContentLoadedEventArgs*)>
        // callFunc;

        // auto callback =
        // wil::MakeAgileCallback<ICoreWebView2DOMContentLoadedEventHandler>(lambda);

        // auto test { callback.Get() };

        ::EventRegistrationToken token2;
        webView.core->add_DOMContentLoaded(
            wil::MakeAgileCallback<ICoreWebView2DOMContentLoadedEventHandler>(
                [](ICoreWebView2* sender,
                   ICoreWebView2DOMContentLoadedEventArgs* args) -> ::HRESULT {
            glow::log::log("TEST");

            return S_OK;
        }).Get(),
            &token2);
    });

    return glow::app::run();
}
