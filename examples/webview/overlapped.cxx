#include <glow/glow.hxx>

glow::window::WebView webView;

auto wWinMain([[maybe_unused]] ::HINSTANCE instance,
              [[maybe_unused]] ::HINSTANCE prevInstance,
              [[maybe_unused]] wchar_t* cmdLine,
              [[maybe_unused]] int cmdShow) -> int {
    webView.messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    webView.create([]() { webView.navigate(u8"http://localhost:5173/"); });

    return glow::message::run_loop();
}
