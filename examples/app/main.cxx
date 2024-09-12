#include <glow/glow.hxx>

#include <unordered_map>

enum struct glow::message::Code : ::UINT {
    CREATE_WINDOW = WM_APP,
    CREATE_FOREGROUND_WINDOW,
    CLOSE_WINDOW,
};
namespace wm = glow::message::wm;

struct App final : glow::app::App {
    App(glow::system::Event& singleInstance)
        : m_singleInstance { singleInstance } {
        messages.on(WM_NOTIFY, [this](wm::NOTIFY msg) {
            auto& idFrom { msg.notification().nmhdr.idFrom };
            auto& code { msg.notification().code };

            using enum glow::message::Code;

            switch (code) {
                case CREATE_WINDOW: {
                    windows.add(std::make_unique<::Window>(m_keys));
                } break;
                case CREATE_FOREGROUND_WINDOW: {
                    notify(CREATE_WINDOW);
                    // glow::window::set_foreground(m_windows.last_window());
                    auto last { windows.last_window() };
                } break;
                case CLOSE_WINDOW: {
                    windows.remove(idFrom);
                } break;
            }

            return 0;
        });

        create();

        m_singleInstance.m_callback = [this]() { notify(CREATE_FOREGROUND_WINDOW); };

        notify(CREATE_WINDOW);
    }

    auto operator()() -> int { return glow::app::run(); }

    glow::system::Event& m_singleInstance;
    // glow::window::WindowManager<Window> m_windows;
    glow::window::Manager<Window> windows;
    std::unordered_map<char, bool> m_keys { { 'N', false }, { 'W', false } };
};

struct Window final : glow::window::Window {
    Window(std::unordered_map<char, bool>& keys)
        : m_keys { keys } {
        messages.on(WM_DESTROY, [](wm::DESTROY msg) {
            msg.quit();

            return 0;
        });

        message(WM_KEYDOWN, [this](glow::message::wm_keydown_keyup message) {
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

        message(WM_KEYUP, [this](glow::message::wm_keydown_keyup message) {
            auto key { message.key() };

            if (m_keys.contains(key)) {
                m_keys.at(key) = false;
            }

            return 0;
        });

        create();
        activate();
    }

    std::unordered_map<char, bool>& m_keys;
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
