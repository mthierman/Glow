// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <Unknwn.h>
#include <dwmapi.h>
#include <gdiplus.h>
#include <wrl.h>

#include <cstdint>
#include <stdexcept>
#include <string>

#include <wil/com.h>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <nlohmann/json.hpp>

#include "console.hxx"
#include "gui.hxx"
#include "text.hxx"

namespace glow
{
namespace window
{
template <typename T> struct MessageWindow
{
    MessageWindow(std::string name = "MessageWindow", intptr_t id = glow::text::random<intptr_t>())
        : m_id{id}
    {
        WNDCLASSEXA wcex{sizeof(WNDCLASSEXA)};

        if (!GetClassInfoExA(GetModuleHandleA(nullptr), name.c_str(), &wcex))
        {
            wcex.lpszClassName = name.c_str();
            wcex.lpszMenuName = 0;
            wcex.lpfnWndProc = WndProc;
            wcex.style = 0;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = sizeof(intptr_t);
            wcex.hInstance = GetModuleHandleA(nullptr);
            wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
            wcex.hCursor = static_cast<HCURSOR>(
                LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
            wcex.hIcon = static_cast<HICON>(
                LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
            wcex.hIconSm = static_cast<HICON>(
                LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));

            if (RegisterClassExA(&wcex) == 0)
                throw std::runtime_error("Class registration failure");
        }

        if (CreateWindowExA(0, wcex.lpszClassName, wcex.lpszClassName, 0, 0, 0, 0, 0, HWND_MESSAGE,
                            nullptr, GetModuleHandleA(nullptr), this) == nullptr)
            throw std::runtime_error("Window creation failure");
    }

    auto hwnd() const -> HWND { return m_hwnd.get(); }

    auto close() -> int
    {
        m_hwnd.reset();

        return 0;
    }

  private:
    static auto CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        T* self{nullptr};

        if (message == WM_NCCREATE)
        {
            auto lpCreateStruct{reinterpret_cast<CREATESTRUCTA*>(lParam)};
            self = static_cast<T*>(lpCreateStruct->lpCreateParams);
            self->m_hwnd.reset(hwnd);
            SetWindowLongPtrA(hwnd, 0, reinterpret_cast<intptr_t>(self));
        }

        else self = reinterpret_cast<T*>(GetWindowLongPtrA(hwnd, 0));

        if (self) { return self->default_wnd_proc(hwnd, message, wParam, lParam); }

        else { return DefWindowProcA(hwnd, message, wParam, lParam); }
    }

    virtual auto default_wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        switch (message)
        {
        case WM_CLOSE: return close();
        case WM_DESTROY: PostQuitMessage(0); return 0;
        }

        return wnd_proc(hwnd, message, wParam, lParam);
    }

    virtual auto wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        return DefWindowProcA(hwnd, message, wParam, lParam);
    }

  public:
    intptr_t m_id;
    wil::unique_hwnd m_hwnd;
};

template <typename T> struct BaseWindow
{
    BaseWindow(std::string name = "BaseWindow", intptr_t id = glow::text::random<intptr_t>(),
               DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, DWORD exStyle = 0,
               int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = CW_USEDEFAULT,
               int height = CW_USEDEFAULT, HWND parent = nullptr, HMENU menu = nullptr)
        : m_id{id}
    {
        WNDCLASSEXA wcex{sizeof(WNDCLASSEXA)};

        if (!GetClassInfoExA(GetModuleHandleA(nullptr), name.c_str(), &wcex))
        {
            wil::unique_hicon appIcon{
                static_cast<HICON>(LoadImageA(GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101),
                                              IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};

            wcex.lpszClassName = name.c_str();
            wcex.lpszMenuName = 0;
            wcex.lpfnWndProc = WndProc;
            wcex.style = 0;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = sizeof(intptr_t);
            wcex.hInstance = GetModuleHandleA(nullptr);
            wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
            wcex.hCursor = static_cast<HCURSOR>(
                LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
            wcex.hIcon = m_icon.get()
                             ? m_icon.get()
                             : static_cast<HICON>(LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON,
                                                             0, 0, LR_SHARED | LR_DEFAULTSIZE));
            wcex.hIconSm = m_icon.get()
                               ? m_icon.get()
                               : static_cast<HICON>(LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON,
                                                               0, 0, LR_SHARED | LR_DEFAULTSIZE));

            if (RegisterClassExA(&wcex) == 0)
                throw std::runtime_error("Class registration failure");
        }

        if (CreateWindowExA(exStyle, wcex.lpszClassName, wcex.lpszClassName, style, x, y, width,
                            height, parent, menu, GetModuleHandleA(nullptr), this) == nullptr)
            throw std::runtime_error("Window creation failure");

        m_scale = scale();
        m_dpi = dpi();
    }

    auto hwnd() const -> HWND { return m_hwnd.get(); }

    auto id() const -> intptr_t { return m_id; }

    auto close() -> int
    {
        m_hwnd.reset();

        return 0;
    }

    auto notify(HWND receiver, unsigned int code = WM_APP, std::string message = "") -> void
    {
        m_notification.nmhdr.code = code;
        m_notification.message = message;
        m_notification.nmhdr.idFrom = id();
        m_notification.nmhdr.hwndFrom = hwnd();

        SendMessageA(receiver, WM_NOTIFY, m_notification.nmhdr.idFrom,
                     reinterpret_cast<uintptr_t>(&m_notification));
    }

    auto dpi() -> int { return GetDpiForWindow(hwnd()); };

    auto scale() -> float
    {
        return static_cast<float>(dpi()) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
    };

    auto reveal() -> void { ShowWindow(hwnd(), SW_SHOWNORMAL); }

    auto show() -> void { ShowWindow(hwnd(), SW_SHOW); }

    auto hide() -> void { ShowWindow(hwnd(), SW_HIDE); }

    auto maximize() -> bool
    {
        auto style{GetWindowLongPtrA(hwnd(), GWL_STYLE)};

        WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
        GetWindowPlacement(hwnd(), &wp);

        if ((style & WS_OVERLAPPEDWINDOW) && wp.showCmd == 3)
        {
            ShowWindow(hwnd(), SW_SHOWNORMAL);

            return false;
        }

        else
        {
            ShowWindow(hwnd(), SW_MAXIMIZE);

            return true;
        }
    }

    auto fullscreen() -> bool
    {
        static RECT pos;

        auto style{GetWindowLongPtrA(hwnd(), GWL_STYLE)};

        if (style & WS_OVERLAPPEDWINDOW)
        {
            MONITORINFO mi{sizeof(mi)};
            GetWindowRect(hwnd(), &pos);
            if (GetMonitorInfoA(MonitorFromWindow(hwnd(), MONITOR_DEFAULTTONEAREST), &mi))
            {
                SetWindowLongPtrA(hwnd(), GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos(hwnd(), HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                             mi.rcMonitor.right - mi.rcMonitor.left,
                             mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED);
            }

            return true;
        }

        else
        {
            SetWindowLongPtrA(hwnd(), GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd(), nullptr, pos.left, pos.top, (pos.right - pos.left),
                         (pos.bottom - pos.top), SWP_FRAMECHANGED);

            return false;
        }
    }

    auto topmost() -> bool
    {
        FLASHWINFO fwi{sizeof(FLASHWINFO)};
        fwi.hwnd = hwnd();
        fwi.dwFlags = FLASHW_CAPTION;
        fwi.uCount = 1;
        fwi.dwTimeout = 100;

        auto style{GetWindowLongPtrA(hwnd(), GWL_EXSTYLE)};

        if (style & WS_EX_TOPMOST)
        {
            SetWindowPos(hwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            FlashWindowEx(&fwi);

            return false;
        }

        else
        {
            SetWindowPos(hwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            FlashWindowEx(&fwi);

            return true;
        }
    }

    auto title(std::string title) -> void { SetWindowTextA(hwnd(), title.c_str()); }

    auto icon(HICON icon) -> void
    {
        SendMessageA(hwnd(), WM_SETICON, ICON_SMALL, static_cast<LPARAM>(icon));
        SendMessageA(hwnd(), WM_SETICON, ICON_BIG, static_cast<LPARAM>(icon));
    }

    auto border(bool enabled) -> void
    {
        auto style{GetWindowLongPtrA(hwnd(), GWL_STYLE)};

        SetWindowLongPtrA(hwnd(), GWL_STYLE, enabled ? (style | WS_BORDER) : (style & ~WS_BORDER));
        SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto overlapped() -> void
    {
        SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
        SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto popup() -> void
    {
        SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
        SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto child() -> void
    {
        SetWindowLongPtrA(hwnd(), GWL_STYLE, WS_CHILD);
        SetWindowPos(hwnd(), nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    auto reparent(HWND parent) -> void
    {
        if (parent)
        {
            SetParent(hwnd(), parent);
            child();
        }

        else
        {
            SetParent(hwnd(), nullptr);
            popup();
        }
    }

    auto client_rect() -> void { GetClientRect(hwnd(), &m_clientRect); }

    auto window_rect() -> void { GetWindowRect(hwnd(), &m_windowRect); }

    auto client_position() -> glow::gui::Position
    {
        auto rect{client_rect()};

        glow::gui::Position p;

        p.x = rect.left;
        p.y = rect.top;
        p.width = rect.right - rect.left;
        p.height = rect.bottom - rect.top;

        return p;
    }

    auto window_position() -> glow::gui::Position
    {
        auto rect{window_rect()};

        glow::gui::Position p;

        p.x = rect.left;
        p.y = rect.top;
        p.width = rect.right - rect.left;
        p.height = rect.bottom - rect.top;

        return p;
    }

    auto dwm_dark_mode(bool enabled) -> void
    {
        if (enabled)
        {
            auto useImmersiveDarkMode{TRUE};
            DwmSetWindowAttribute(hwnd(), DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
                                  sizeof(useImmersiveDarkMode));
        }

        else
        {
            auto useImmersiveDarkMode{FALSE};
            DwmSetWindowAttribute(hwnd(), DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode,
                                  sizeof(useImmersiveDarkMode));
        }
    }

    auto dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE backdrop) -> void
    {
        // MARGINS m{-1};
        MARGINS m{0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
        if (FAILED(DwmExtendFrameIntoClientArea(m_hwnd.get(), &m))) return;

        if (FAILED(DwmSetWindowAttribute(m_hwnd.get(), DWMWA_SYSTEMBACKDROP_TYPE, &backdrop,
                                         sizeof(&backdrop))))
            return;
    }

    auto dwm_rounded_corners(bool enable) -> void
    {
        if (enable)
        {
            auto corner{DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_ROUND};
            DwmSetWindowAttribute(hwnd(), DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
        }

        else
        {
            auto corner{DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DONOTROUND};
            DwmSetWindowAttribute(hwnd(), DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
        }
    }

    auto dwm_cloak(bool enable) -> void
    {
        if (enable)
        {
            auto cloak{TRUE};
            DwmSetWindowAttribute(hwnd(), DWMWA_CLOAK, &cloak, sizeof(cloak));
        }

        else
        {
            auto cloak{FALSE};
            DwmSetWindowAttribute(hwnd(), DWMWA_CLOAK, &cloak, sizeof(cloak));
        }
    }

    auto dwm_caption_color(bool enable) -> void
    {
        if (enable)
        {
            auto captionColor{DWMWA_COLOR_DEFAULT};
            DwmSetWindowAttribute(hwnd(), DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));
        }

        else
        {
            auto captionColor{DWMWA_COLOR_NONE};
            DwmSetWindowAttribute(hwnd(), DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));
        }
    }

    auto dwm_set_caption_color(COLORREF color) -> void
    {
        DwmSetWindowAttribute(hwnd(), DWMWA_CAPTION_COLOR, &color, sizeof(color));
    }

    auto dwm_border_color(bool enable) -> void
    {
        if (enable)
        {
            auto borderColor{DWMWA_COLOR_DEFAULT};
            DwmSetWindowAttribute(hwnd(), DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
        }

        else
        {
            auto borderColor{DWMWA_COLOR_NONE};
            DwmSetWindowAttribute(hwnd(), DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
        }
    }

    auto dwm_set_border_color(COLORREF color) -> void
    {
        DwmSetWindowAttribute(hwnd(), DWMWA_BORDER_COLOR, &color, sizeof(color));
    }

    auto dwm_set_text_color(COLORREF color) -> void
    {
        DwmSetWindowAttribute(hwnd(), DWMWA_TEXT_COLOR, &color, sizeof(color));
    }

    auto dwm_reset_text_color() -> void
    {
        auto textColor{DWMWA_COLOR_DEFAULT};
        DwmSetWindowAttribute(hwnd(), DWMWA_TEXT_COLOR, &textColor, sizeof(textColor));
    }

  private:
    static auto CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        T* self{nullptr};

        if (message == WM_NCCREATE)
        {
            auto lpCreateStruct{reinterpret_cast<CREATESTRUCTA*>(lParam)};
            self = static_cast<T*>(lpCreateStruct->lpCreateParams);
            self->m_hwnd.reset(hwnd);
            SetWindowLongPtrA(hwnd, 0, reinterpret_cast<intptr_t>(self));
        }

        else self = reinterpret_cast<T*>(GetWindowLongPtrA(hwnd, 0));

        if (self) { return self->default_wnd_proc(hwnd, message, wParam, lParam); }

        else { return DefWindowProcA(hwnd, message, wParam, lParam); }
    }

    virtual auto default_wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        switch (message)
        {
        case WM_CLOSE: return close();
        }

        return wnd_proc(hwnd, message, wParam, lParam);
    }

    virtual auto wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        return DefWindowProcA(hwnd, message, wParam, lParam);
    }

  public:
    RECT m_clientRect{};
    RECT m_windowRect{};
    int m_dpi{};
    float m_scale{};
    glow::gui::Position m_position;
    glow::gui::Notification m_notification;
    wil::unique_hwnd m_hwnd;
    intptr_t m_id{};
    wil::unique_hicon m_icon{static_cast<HICON>(LoadImageA(
        GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};
};

template <typename T> struct WebView : BaseWindow<T>
{
    T* self{static_cast<T*>(this)};

    WebView(HWND parent, intptr_t id = glow::text::random<intptr_t>())
        : BaseWindow<T>("WebView", id, WS_CHILD, 0, 0, 0, 0, 0, parent, reinterpret_cast<HMENU>(id))
    {
        m_parent = parent;
        glow::console::hresult_check(create());
    }

    auto wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT override
    {
        switch (message)
        {
        case WM_SIZE: return on_size(hwnd, wParam, lParam);
        }

        return DefWindowProcA(hwnd, message, wParam, lParam);
    }

    auto on_size(HWND hwnd, WPARAM wParam, LPARAM lParam) -> int
    {
        if (m_webView.controller4)
        {
            self->client_rect();
            m_webView.controller4->put_Bounds(self->m_clientRect);
        }

        return 0;
    }

    auto create() -> HRESULT
    {
        return CreateCoreWebView2EnvironmentWithOptions(
            nullptr, nullptr, nullptr,
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [=, this](HRESULT errorCode,
                          ICoreWebView2Environment* createdEnvironment) -> HRESULT
                {
                    if (createdEnvironment)
                    {
                        glow::console::hresult_check(create_controller(createdEnvironment));
                    }

                    return errorCode;
                })
                .Get());
    }

    auto create_controller(ICoreWebView2Environment* createdEnvironment) -> HRESULT
    {
        return createdEnvironment->CreateCoreWebView2Controller(
            self->hwnd(),
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                [=, this](HRESULT errorCode, ICoreWebView2Controller* createdController) -> HRESULT
                {
                    if (createdController)
                    {
                        m_webView.controller = createdController;
                        m_webView.controller4 =
                            m_webView.controller.try_query<ICoreWebView2Controller4>();

                        if (!m_webView.controller4) return E_POINTER;

                        COREWEBVIEW2_COLOR bgColor{0, 0, 0, 0};
                        glow::console::hresult_check(
                            m_webView.controller4->put_DefaultBackgroundColor(bgColor));

                        // m_webView.controller4->navigate("https://www.bing.com/");
                        navigate("https://www.bing.com/");

                        SendMessageA(m_parent, WM_SIZE, 0, 0);
                        SendMessageA(self->hwnd(), WM_SIZE, 0, 0);

                        glow::console::hresult_check(
                            m_webView.controller->get_CoreWebView2(m_webView.core.put()));
                        m_webView.core20 = m_webView.core.try_query<ICoreWebView2_20>();

                        if (!m_webView.core20) return E_POINTER;

                        glow::console::hresult_check(
                            m_webView.core20->get_Settings(m_webView.settings.put()));

                        m_webView.settings8 =
                            m_webView.settings.try_query<ICoreWebView2Settings8>();

                        if (!m_webView.settings8) return E_POINTER;

                        glow::console::hresult_check(settings());

                        glow::console::hresult_check(context_menu_requested());
                        glow::console::hresult_check(source_changed());
                        glow::console::hresult_check(navigation_starting());
                        glow::console::hresult_check(navigation_completed());
                        glow::console::hresult_check(web_message_received());
                        glow::console::hresult_check(accelerator_key_pressed());
                        glow::console::hresult_check(favicon_changed());
                        glow::console::hresult_check(document_title_changed());
                        glow::console::hresult_check(zoom_factor_changed());

                        initialized();
                    }

                    return errorCode;
                })
                .Get());
    }

    virtual auto settings() -> HRESULT
    {
        glow::console::hresult_check(
            m_webView.settings8->put_AreBrowserAcceleratorKeysEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_AreDefaultContextMenusEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_AreDefaultScriptDialogsEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_AreDevToolsEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_AreHostObjectsAllowed(true));
        glow::console::hresult_check(m_webView.settings8->put_HiddenPdfToolbarItems(
            COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE));
        glow::console::hresult_check(m_webView.settings8->put_IsBuiltInErrorPageEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsGeneralAutofillEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsPasswordAutosaveEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsPinchZoomEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsReputationCheckingRequired(true));
        glow::console::hresult_check(m_webView.settings8->put_IsScriptEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsStatusBarEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsSwipeNavigationEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsWebMessageEnabled(true));
        glow::console::hresult_check(m_webView.settings8->put_IsZoomControlEnabled(true));

        return S_OK;
    }

    virtual auto initialized() -> void {}

    auto context_menu_requested() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_ContextMenuRequested(
            Microsoft::WRL::Callback<ICoreWebView2ContextMenuRequestedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2ContextMenuRequestedEventArgs* args) -> HRESULT
                { return context_menu_requested_handler(sender, args); })
                .Get(),
            &token);
    }

    auto source_changed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_SourceChanged(
            Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
                { return source_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto navigation_starting() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_NavigationStarting(
            Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT
                { return navigation_starting_handler(sender, args); })
                .Get(),
            &token);
    }

    auto navigation_completed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_NavigationCompleted(
            Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
                { return navigation_completed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto web_message_received() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_WebMessageReceived(
            Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                [=, this](ICoreWebView2* sender,
                          ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                { return web_message_received_handler(sender, args); })
                .Get(),
            &token);
    }

    auto document_title_changed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_DocumentTitleChanged(
            Microsoft::WRL::Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                [=, this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                { return document_title_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto favicon_changed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.core20->add_FaviconChanged(
            Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                [=, this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                { return favicon_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto accelerator_key_pressed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.controller4->add_AcceleratorKeyPressed(
            Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
                [=, this](ICoreWebView2Controller* sender,
                          ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT
                { return accelerator_key_pressed_handler(sender, args); })
                .Get(),
            &token);
    }

    auto zoom_factor_changed() -> HRESULT
    {
        EventRegistrationToken token;

        return m_webView.controller4->add_ZoomFactorChanged(
            Microsoft::WRL::Callback<ICoreWebView2ZoomFactorChangedEventHandler>(
                [=, this](ICoreWebView2Controller* sender, IUnknown* args) -> HRESULT
                { return zoom_factor_changed_handler(sender, args); })
                .Get(),
            &token);
    }

    virtual auto context_menu_requested_handler(ICoreWebView2* sender,
                                                ICoreWebView2ContextMenuRequestedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }

    virtual auto source_changed_handler(ICoreWebView2* sender,
                                        ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
    {
        return S_OK;
    }

    virtual auto navigation_starting_handler(ICoreWebView2* sender,
                                             ICoreWebView2NavigationStartingEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }

    virtual auto navigation_completed_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationCompletedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }

    virtual auto web_message_received_handler(ICoreWebView2* sender,
                                              ICoreWebView2WebMessageReceivedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }

    virtual auto document_title_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
    {
        return S_OK;
    }

    virtual auto favicon_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
    {
        return S_OK;
    }

    virtual auto accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                                 ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }

    virtual auto zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
        -> HRESULT
    {
        return S_OK;
    }

    auto navigate(std::string url) -> HRESULT
    {
        if (m_webView.core20)
        {
            auto wideUrl{glow::text::widen(url)};
            if (wideUrl.empty()) return S_OK;
            else return glow::console::hresult_check(m_webView.core20->Navigate(wideUrl.c_str()));
        }

        else return S_OK;
    }

    auto post_json(const nlohmann::json message) -> HRESULT
    {
        if (m_webView.core20)
        {
            auto wideJson{glow::text::widen(message.dump())};
            if (wideJson.empty()) return S_OK;
            else
                return glow::console::hresult_check(
                    m_webView.core20->PostWebMessageAsJson(wideJson.c_str()));
        }

        else return S_OK;
    }

    HWND m_parent{nullptr};

    struct WebView2
    {
        wil::com_ptr<ICoreWebView2EnvironmentOptions6> evironmentOptions6;
        wil::com_ptr<ICoreWebView2Controller> controller;
        wil::com_ptr<ICoreWebView2Controller4> controller4;
        wil::com_ptr<ICoreWebView2> core;
        wil::com_ptr<ICoreWebView2_20> core20;
        wil::com_ptr<ICoreWebView2Settings> settings;
        wil::com_ptr<ICoreWebView2Settings8> settings8;
    };
    WebView2 m_webView;
};
} // namespace window
} // namespace glow
