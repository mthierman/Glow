#include <glow/glow.hxx>

struct WebView final : glow::window::WebView {
    WebView() {
        messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        create([this]() {
            if (core) {
                core->add_DocumentTitleChanged(
                    event.make<ICoreWebView2DocumentTitleChangedEventHandler>(
                        [this]([[maybe_unused]] ICoreWebView2* sender,
                               [[maybe_unused]] IUnknown* args) {
                    if (auto title { document_title() }; title) {
                        set_title(title.value());
                    }

                    return S_OK;
                }),
                    event.token(u8"DocumentTitleChanged"));
            }

            navigate(u8"http://localhost:5173/");
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
