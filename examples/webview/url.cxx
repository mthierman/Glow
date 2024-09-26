#include <glow/glow.hxx>

struct WebView final : glow::window::WebView {
    WebView() {
        messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        create([this]() {
            if (core) {
                core->add_SourceChanged(
                    event.make<ICoreWebView2SourceChangedEventHandler>(
                        [this](ICoreWebView2* sender,
                               ICoreWebView2SourceChangedEventArgs* args [[maybe_unused]]) {
                    wil::unique_cotaskmem_string uri;
                    sender->get_Source(&uri);
                    set_title(uri.get());

                    return S_OK;
                }),
                    event.token("SourceChanged"));
            }

            // navigate("http://localhost:5173/");
            // navigate("file:///D:/Repos/mthierman/Glow/data/test.html");
            auto testHtml { LR"(<!doctype html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <meta name="color-scheme" content="dark light" />
        <title>Glow</title>
    </head>
    <body style="background-color: black">
        <p style="color: white">test</p>
    </body>
</html>
)" };
            navigate_to_string(testHtml);
        });
    }
};

auto main() -> int {
    WebView webView;

    return glow::message::run_loop();
}
