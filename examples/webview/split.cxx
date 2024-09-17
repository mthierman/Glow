#include <glow/glow.hxx>

struct WebView final : glow::window::WebView { };

struct Window final : glow::window::Overlapped {
    Window() {
        messages.on(WM_CREATE, [this](glow::message::wm::CREATE /* msg */) {
            left.create(hwnd.get(), [this]() { left.navigate("http://localhost:5173/"); }, true);
            right.create(hwnd.get(), [this]() { right.navigate("http://localhost:5173/"); }, true);

            return 0;
        });

        messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        messages.on(WM_WINDOWPOSCHANGED, [this](glow::message::wm::WINDOWPOSCHANGED /* msg */) {
            wil::for_each_child_window(hwnd.get(), [this](::HWND childHwnd) {
                auto id { ::GetWindowLongPtrW(childHwnd, GWL_ID) };

                if (id == left.get_id()) {
                    ::SetWindowPos(childHwnd,
                                   nullptr,
                                   0,
                                   0,
                                   client_position().width / 2,
                                   client_position().height,
                                   SWP_NOZORDER | SWP_NOACTIVATE);
                }

                if (id == right.get_id()) {
                    ::SetWindowPos(childHwnd,
                                   nullptr,
                                   client_position().width / 2,
                                   0,
                                   client_position().width / 2,
                                   client_position().height,
                                   SWP_NOZORDER | SWP_NOACTIVATE);
                }

                return true;
            });

            return 0;
        });

        create(true);
    }

    WebView left;
    WebView right;
};

auto main() -> int {
    Window window;

    return glow::message::run_loop();
}
