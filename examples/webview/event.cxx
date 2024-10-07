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
                        [this](ICoreWebView2* sender [[maybe_unused]],
                               IUnknown* args [[maybe_unused]]) {
                    if (document_title().has_value()) {
                        set_title(document_title().value());
                    }

                    return S_OK;
                }),
                    event.token(u8"DocumentTitleChanged"));
            }

            navigate(u8"http://localhost:5173/");
        });
    }
};

auto main() -> int {
    WebView webView;

    return glow::message::run_loop();
}
