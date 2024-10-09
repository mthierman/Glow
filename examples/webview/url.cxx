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
                        [this]([[maybe_unused]] ICoreWebView2* sender,
                               [[maybe_unused]] ICoreWebView2SourceChangedEventArgs* args) {
                    if (auto src { source() }; src) {
                        set_title(src.value());
                    }

                    return S_OK;
                }),
                    event.token(u8"SourceChanged"));
            }

            // navigate("http://localhost:5173/");

            // navigate("file:///D:/Repos/mthierman/Glow/data/test.html");

            //             auto testHtml { LR"(<!doctype html>
            // <html lang="en">
            //     <head>
            //         <meta charset="UTF-8" />
            //         <meta name="viewport" content="width=device-width, initial-scale=1.0" />
            //         <meta name="color-scheme" content="dark light" />
            //         <title>Glow</title>
            //     </head>
            //     <body style="background-color: black">
            //         <p style="color: white">test</p>
            //     </body>
            // </html>
            // )" };
            //             navigate_to_string(testHtml);

            // virtual_host_name_mapping("glow", glow::filesystem::known_folder());
            // navigate("https://glow/Glow/test.html");
        });
    }
};

auto wWinMain([[maybe_unused]] ::HINSTANCE instance,
              [[maybe_unused]] ::HINSTANCE prevInstance,
              [[maybe_unused]] wchar_t* cmdLine,
              [[maybe_unused]] int cmdShow) -> int {
    WebView webView;

    return glow::message::run_loop();
}
