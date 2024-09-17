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

                auto clientPosition { client_position() };

                if (id == left.get_id()) {
                    left.set_position(glow::window::Position(
                        0, 0, clientPosition.width / 2, clientPosition.height));
                }

                if (id == right.get_id()) {
                    right.set_position(glow::window::Position(clientPosition.width / 2,
                                                              0,
                                                              clientPosition.width / 2,
                                                              clientPosition.height));
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
