#include <glow/glow.hxx>

#include <unordered_map>

enum struct glow::message::Code : ::UINT {
    CREATE_WINDOW = WM_APP,
    CREATE_FOREGROUND_WINDOW,
    CLOSE_WINDOW,
};

struct Window final : glow::window::Overlapped {
    using enum glow::message::Code;

    Window(std::unordered_map<char, bool>& keyMap)
        : keys { keyMap } {
        messages.on(WM_KEYDOWN, [this](glow::message::wm::KEYDOWN message) {
            if (glow::input::was_key_down(VK_CONTROL)) {
                switch (auto key { message.key() }; key) {
                    case 'N': {
                        if (!keys.at(key)) {
                            keys.at(key) = true;
                            notify_app(CREATE_WINDOW);
                        }
                        break;
                    }

                    case 'W': {
                        if (!keys.at(key)) {
                            keys.at(key) = true;
                            notify_app(CLOSE_WINDOW);
                        }
                        break;
                    }
                }
            }

            return 0;
        });

        messages.on(WM_KEYUP, [this](glow::message::wm::KEYUP message) {
            auto key { message.key() };

            if (keys.contains(key)) {
                keys.at(key) = false;
            }

            return 0;
        });

        create();
        activate();
    }

    std::unordered_map<char, bool>& keys;
};

struct App final : glow::window::Message {
    using enum glow::message::Code;

    App(glow::system::Event& singleInstance)
        : singleInstance { singleInstance } {
        messages.on(WM_NOTIFY, [this](glow::message::wm::NOTIFY msg) {
            auto& idFrom { msg.notification().nmhdr.idFrom };
            auto& code { msg.notification().code };

            switch (code) {
                case CREATE_WINDOW: {
                    windows.add(std::make_unique<::Window>(keys));
                } break;
                case CREATE_FOREGROUND_WINDOW: {
                    notify_app(CREATE_WINDOW);

                    if (auto last { windows.last() }; last) {
                        last->bring_to_top();
                    }
                } break;
                case CLOSE_WINDOW: {
                    windows.remove(idFrom);
                } break;
            }

            return 0;
        });

        create();

        singleInstance.m_callback = [this]() { notify_app(CREATE_FOREGROUND_WINDOW); };

        notify_app(CREATE_WINDOW);
    }

    glow::system::Event& singleInstance;
    glow::window::Manager<Window> windows;
    std::unordered_map<char, bool> keys { { 'N', false }, { 'W', false } };
};

auto main() -> int {
    auto coInit { glow::system::co_initialize() };

    glow::system::Event singleInstance;

    if (singleInstance.create("SingleInstance")) {
        return EXIT_SUCCESS;
    }

    App app(singleInstance);

    return app();
}
