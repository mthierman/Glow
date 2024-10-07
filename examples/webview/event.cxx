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
                        [this](ICoreWebView2* sender, IUnknown* args [[maybe_unused]]) {
                    wil::unique_cotaskmem_string title;
                    sender->get_DocumentTitle(&title);
                    set_title(glow::text::to_u8string(title.get()));

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
