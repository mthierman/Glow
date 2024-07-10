#include <glow/input.hxx>
#include <glow/messages.hxx>
#include <glow/window.hxx>

#include <unordered_map>

namespace glow::messages {
enum struct notice : unsigned int {
    CREATE_WINDOW = WM_APP,
    CREATE_FOREGROUND_WINDOW,
    CLOSE_WINDOW,
};
}
using enum glow::messages::notice;

struct Window final : glow::window::Window {
    Window(std::unordered_map<char, bool>& keys)
        : m_keys { keys } {
        message(WM_CLOSE, [this](glow::messages::wm message) {
            notify(CLOSE_WINDOW);
            return 0;
        });

        message(WM_KEYDOWN, [this](glow::messages::wm_keydown_keyup message) {
            if (glow::input::was_key_down(VK_CONTROL)) {
                switch (auto key { message.key() }; key) {
                    case 'N': {
                        if (!m_keys.at(key)) {
                            m_keys.at(key) = true;
                            notify(CREATE_WINDOW);
                        }
                        break;
                    }

                    case 'W': {
                        if (!m_keys.at(key)) {
                            m_keys.at(key) = true;
                            notify(CLOSE_WINDOW);
                        }
                        break;
                    }
                }
            }

            return 0;
        });

        message(WM_KEYUP, [this](glow::messages::wm_keydown_keyup message) {
            auto key { message.key() };

            if (m_keys.contains(key)) {
                m_keys.at(key) = false;
            }

            return 0;
        });

        create();
    }

    std::unordered_map<char, bool>& m_keys;
};

struct App final : glow::window::Window {
    App(glow::system::Event& singleInstance)
        : m_singleInstance { singleInstance } {
        message(WM_NOTIFY, [this](glow::messages::wm_notify message) {
            auto& notification { message.notification() };

            if (notification.notice == CREATE_WINDOW) {
                m_windows.add(std::make_unique<::Window>(m_keys));
            }

            if (notification.notice == CREATE_FOREGROUND_WINDOW) {
                notify(CREATE_WINDOW);
                glow::window::set_foreground(m_windows.last_window());
            }

            if (notification.notice == CLOSE_WINDOW) {
                m_windows.remove(notification.idFrom);
            }

            return 0;
        });

        create_message_only();

        m_singleInstance.m_callback = [this]() { notify(CREATE_FOREGROUND_WINDOW); };

        notify(CREATE_WINDOW);
    }

    auto operator()() -> int { return glow::messages::run_loop(); }

    glow::system::Event& m_singleInstance;
    glow::window::WindowManager<Window> m_windows;
    std::unordered_map<char, bool> m_keys { { 'N', false }, { 'W', false } };
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
