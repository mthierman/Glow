#include <glow/glow.hxx>

struct WebView final : glow::window::WebView { };

struct Window final : glow::window::Overlapped {
    Window() {
        messages.on(WM_CREATE, [this](glow::message::wm::CREATE /* msg */) {
            left.create(hwnd.get(), [this]() { left.navigate("https://localhost:5173/"); }, true);
            right.create(hwnd.get(), [this]() { right.navigate("https://localhost:5173/"); }, true);

            return 0;
        });

        messages.on(WM_DESTROY, [](glow::message::wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        messages.on(WM_WINDOWPOSCHANGED, [this](glow::message::wm::WINDOWPOSCHANGED /* msg */) {
            wil::for_each_child_window(hwnd.get(), [this](::HWND hwnd) {
                auto id { ::GetWindowLongPtrW(hwnd, GWL_ID) };

                if (id == left.get_id()) {
                    ::SetWindowPos(hwnd,
                                   nullptr,
                                   0,
                                   0,
                                   client_rect().right / 2,
                                   client_rect().bottom,
                                   SWP_NOZORDER | SWP_NOACTIVATE);
                }

                if (id == right.get_id()) {
                    ::SetWindowPos(hwnd,
                                   nullptr,
                                   client_rect().right / 2,
                                   0,
                                   client_rect().right / 2,
                                   client_rect().bottom,
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

    return glow::app::run();
}
