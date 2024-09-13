#include <glow/glow.hxx>

#include <wil/windowing.h>

namespace wm = glow::message::wm;
glow::window::Overlapped overlapped;
glow::window::WebView webView;

auto main() -> int {
    overlapped.messages.on(WM_DESTROY, [](wm::DESTROY msg) {
        msg.quit();

        return 0;
    });

    overlapped.messages.on(WM_WINDOWPOSCHANGED, [](wm::WINDOWPOSCHANGED /* msg */) {
        wil::for_each_child_window(overlapped.hwnd.get(), [](::HWND hwnd) {
            auto id { ::GetWindowLongPtrW(hwnd, GWL_ID) };
            glow::log::log("{}", id);
            if (id == webView.get_id()) {
                ::SetWindowPos(hwnd,
                               nullptr,
                               0,
                               0,
                               overlapped.client_rect().right / 2,
                               overlapped.client_rect().bottom,
                               0);
            }

            return true;
        });

        return 0;
    });

    overlapped.create();
    overlapped.activate();

    webView.create(overlapped.hwnd.get(),
                   []() { webView.navigate("https://mthierman.pages.dev/"); });
    webView.put_bounds(overlapped.client_rect());

    // webView.create(overlapped.hwnd.get());
    // webView.create();

    return glow::app::run();
}
