// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "app.hxx"

namespace glow
{
App::App(std::string name, size_t id) : m_name{name}, m_id{id} { register_class(); }

App::~App() {}

auto App::register_class() -> void
{
    if (!::GetClassInfoExA(::GetModuleHandleA(nullptr), m_name.c_str(), &m_wcex))
    {
        m_wcex.lpszClassName = m_name.c_str();
        m_wcex.lpszMenuName = nullptr;
        m_wcex.lpfnWndProc = StaticWndProc;
        m_wcex.style = 0;
        m_wcex.cbClsExtra = 0;
        m_wcex.cbWndExtra = sizeof(intptr_t);
        m_wcex.hInstance = ::GetModuleHandleA(nullptr);
        m_wcex.hbrBackground = nullptr;
        m_wcex.hCursor = nullptr;
        m_wcex.hIcon = nullptr;
        m_wcex.hIconSm = nullptr;

        if (::RegisterClassExA(&m_wcex) == 0)
        {
            throw std::runtime_error("App class registration failure");
        }
    }
}

auto App::operator()() -> int
{
    if (::CreateWindowExA(0, m_wcex.lpszClassName, m_wcex.lpszClassName, 0, 0, 0, 0, 0,
                          HWND_MESSAGE, nullptr, ::GetModuleHandleA(nullptr), this) == nullptr)
    {
        throw std::runtime_error("App creation failure");
    }

    ::MSG msg{};
    int r{};

    while ((r = ::GetMessageA(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) { return -1; }

        else
        {
            ::TranslateMessage(&msg);
            ::DispatchMessageA(&msg);
        }
    }

    return 0;
}

auto App::notify(::HWND receiver, CODE code, std::string message) -> void
{
    m_notification.reset();

    m_notification.nmhdr.hwndFrom = m_hwnd.get();
    m_notification.hwnd = m_hwnd.get();

    m_notification.nmhdr.idFrom = m_id;
    m_notification.id = m_id;

    m_notification.nmhdr.code = std::to_underlying(code);
    m_notification.code = code;

    m_notification.message = message;

    ::SendMessageA(receiver, WM_NOTIFY, reinterpret_cast<uintptr_t>(&m_notification.id),
                   reinterpret_cast<uintptr_t>(&m_notification));
}

auto App::close() -> void { ::SendMessageA(m_hwnd.get(), WM_CLOSE, 0, 0); }

auto CALLBACK App::StaticWndProc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam)
    -> ::LRESULT
{
    auto self{instance_from_wnd_proc<App>(hWnd, uMsg, lParam)};

    if (self)
    {
        switch (uMsg)
        {
            case WM_CLOSE: return self->on_close(wParam, lParam);
            case WM_DESTROY: return self->on_destroy(wParam, lParam);
            default: return self->WndProc(uMsg, wParam, lParam);
        }
    }

    else { return ::DefWindowProcA(hWnd, uMsg, wParam, lParam); }
}

auto App::WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    return ::DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto App::on_close(::WPARAM wParam, ::LPARAM lParam) -> int
{
    m_hwnd.reset();

    return 0;
}

auto App::on_destroy(::WPARAM wParam, ::LPARAM lParam) -> int
{
    ::PostQuitMessage(0);

    return 0;
}
} // namespace glow
