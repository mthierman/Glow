// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/window.hxx>

#include <stdexcept>
#include <utility>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.UI.ViewManagement.h>

#include <glow/log.hxx>
#include <glow/system.hxx>

#include <iostream>

namespace glow::window {
Position::Position(int x, int y, int width, int height)
    : x { x },
      y { y },
      width { width },
      height { height } { }

Position::Position(const ::RECT& rect)
    : x { rect.left },
      y { rect.top },
      width { rect.right - rect.left },
      height { rect.bottom - rect.top } { }

Position::Position(const ::SIZE& size)
    : x { 0 },
      y { 0 },
      width { size.cx },
      height { size.cy } { }

Position::Position(const ::WINDOWPOS& windowPos)
    : x { windowPos.x },
      y { windowPos.y },
      width { windowPos.cx },
      height { windowPos.cy } { }

auto Position::rect() const -> ::RECT {
    return ::RECT { .left { x }, .top { y }, .right { width }, .bottom { height } };
}

Window::Window() {
    baseMessages.on(WM_CREATE, [this](glow::message::wm::CREATE /* msg */) {
        theme_refresh();
        dpi_refresh();

        return 0;
    });

    baseMessages.on(WM_SETTINGCHANGE, [this](glow::message::wm::MSG /* msg */) {
        theme_refresh();
        background_refresh();

        return 0;
    });

    defaultMessages.on(WM_GETMINMAXINFO, [this](glow::message::wm::GETMINMAXINFO msg) {
        auto minMaxInfo { msg.minMaxInfo() };

        if (minTrackSize.x != 0) {
            minMaxInfo->ptMinTrackSize.x = minTrackSize.x;
        }

        if (minTrackSize.y != 0) {
            minMaxInfo->ptMinTrackSize.y = minTrackSize.y;
        }

        if (maxTrackSize.x != 0) {
            minMaxInfo->ptMaxTrackSize.x = maxTrackSize.x;
        }

        if (maxTrackSize.y != 0) {
            minMaxInfo->ptMaxTrackSize.y = maxTrackSize.y;
        }

        return 0;
    });

    baseMessages.on(WM_WINDOWPOSCHANGED, [this](glow::message::wm::WINDOWPOSCHANGED msg) {
        auto windowPos { msg.windowPos() };

        positions.window = Position(*windowPos);
        positions.client = client_position();

        get_window_placement();

        get_monitor_info();
        positions.monitor = Position(monitorInfo.rcMonitor);
        positions.work = Position(monitorInfo.rcWork);

        return 0;
    });

    baseMessages.on(WM_DPICHANGED, [this](glow::message::wm::DPICHANGED msg) {
        dpi_refresh();
        set_position(*msg.suggestedRect());

        return 0;
    });

    defaultMessages.on(WM_ERASEBKGND, [this](glow::message::wm::ERASEBKGND msg) {
        auto hdc { msg.deviceContext() };

        using enum Background::Style;

        switch (background.style) {
            case Transparent: {
                paint_background(hdc, background.brush.transparent);
            } break;
            case System: {
                paint_background(hdc,
                                 state.darkMode ? background.brush.dark : background.brush.light);
            } break;
            case Custom: {
                paint_background(hdc, background.brush.custom);
            } break;
        }

        return 1;
    });

    defaultMessages.on(WM_CLOSE, [this](glow::message::wm::MSG /* msg */) {
        close();

        return 0;
    });
}

auto CALLBACK Window::procedure(::HWND hwnd, ::UINT msg, ::WPARAM wparam, ::LPARAM lparam)
    -> ::LRESULT {
    if (msg == WM_NCCREATE) {
        auto create { reinterpret_cast<::CREATESTRUCTW*>(lparam) };

        if (auto self { static_cast<Window*>(create->lpCreateParams) }; self) {
            ::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<::LONG_PTR>(self));
            self->hwnd.reset(hwnd);
        }
    }

    if (auto self { reinterpret_cast<Window*>(::GetWindowLongPtrW(hwnd, 0)) }; self) {
        if (self->baseMessages.contains(msg)) {
            self->baseMessages.invoke({ hwnd, msg, wparam, lparam });
        }

        if (self->derivedMessages.contains(msg)) {
            self->derivedMessages.invoke({ hwnd, msg, wparam, lparam });
        }

        if (self->messages.contains(msg)) {
            return self->messages.invoke({ hwnd, msg, wparam, lparam });
        }

        if (self->defaultMessages.contains(msg)) {
            return self->defaultMessages.invoke({ hwnd, msg, wparam, lparam });
        }
    }

    return glow::message::default_procedure({ hwnd, msg, wparam, lparam });
}

auto Window::theme_refresh() -> void {
    state.darkMode = glow::system::is_dark();
    state.darkMode ? enable_dark_mode() : disable_dark_mode();
}

auto Window::background_refresh() -> void {
    ::RedrawWindow(hwnd.get(), nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_ERASENOW);
}

auto Window::caption_refresh() -> void {
    switch (background.style) {
        case Background::Style::System: {
            auto dark { glow::system::is_dark() };
            caption_color(dark ? background.color.dark : background.color.light);
            border_color(dark ? background.color.dark : background.color.light);
            text_color(dark ? background.color.light : background.color.dark);
        } break;
        case Background::Style::Custom: {
            auto dark { background.color.custom.is_dark() };
            caption_color(background.color.custom);
            border_color(background.color.custom);
            dark ? text_color(glow::color::Color(255, 255, 255))
                 : text_color(glow::color::Color(0, 0, 0));
        } break;
        case Background::Style::Transparent: {
        } break;
    }
}

auto Window::frame_refresh() -> void {
    ::SetWindowPos(hwnd.get(),
                   nullptr,
                   0,
                   0,
                   0,
                   0,
                   SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED
                       | SWP_SHOWWINDOW);
}

auto Window::window_refresh() -> void {
    background_refresh();
    caption_refresh();
}

auto Window::dpi_refresh() -> void {
    dpi = static_cast<size_t>(::GetDpiForWindow(hwnd.get()));
    scale = (static_cast<double>(dpi) / static_cast<double>(USER_DEFAULT_SCREEN_DPI));
}

auto Window::paint_background(::HDC hdc, const wil::unique_hbrush& brush) -> void {
    if (brush) {
        ::RECT rect;
        ::GetClientRect(hwnd.get(), &rect);
        ::FillRect(hdc, &rect, brush.get());
    }
}

auto Window::caption_color(const glow::color::Color& color) -> void {
    auto colorref { color.colorref() };
    ::DwmSetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR, &colorref, sizeof(colorref));
}

auto Window::text_color(const glow::color::Color& color) -> void {
    auto colorref { color.colorref() };
    ::DwmSetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_TEXT_COLOR, &colorref, sizeof(colorref));
}

auto Window::border_color(const glow::color::Color& color) -> void {
    auto colorref { color.colorref() };
    ::DwmSetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, &colorref, sizeof(colorref));
}

auto Window::background_style(Background::Style style) -> void {
    background.style = style;
    background_refresh();
}

auto Window::background_dark(glow::color::Color color) -> void {
    background.brush.dark.reset(color.brush());
    background.color.dark = color;
    background_refresh();
}

auto Window::background_light(glow::color::Color color) -> void {
    background.brush.light.reset(color.brush());
    background.color.light = color;
    background_refresh();
}

auto Window::background_custom(glow::color::Color color) -> void {
    background.brush.custom.reset(color.brush());
    background.color.custom = color;
    background_refresh();
}

auto Window::activate() -> void {
    cloak();
    ::ShowWindow(hwnd.get(), SW_NORMAL);
    uncloak();
}

auto Window::close() -> void {
    hwnd.reset();
    ::SetWindowLongPtrW(hwnd.get(), 0, reinterpret_cast<::LONG_PTR>(nullptr));
}

auto Window::show() -> void { ::ShowWindow(hwnd.get(), SW_SHOW); }

auto Window::hide() -> void { ::ShowWindow(hwnd.get(), SW_HIDE); }

auto Window::maximize() -> void { ::ShowWindow(hwnd.get(), SW_MAXIMIZE); }

auto Window::minimize() -> void { ::ShowWindow(hwnd.get(), SW_MINIMIZE); }

auto Window::restore() -> void { ::ShowWindow(hwnd.get(), SW_RESTORE); }

auto Window::set_overlapped_window() -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
    frame_refresh();
}

auto Window::is_overlapped_window() -> bool { return get_style() & WS_OVERLAPPEDWINDOW; }

auto Window::set_popup_window() -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, WS_POPUPWINDOW | WS_CLIPCHILDREN);
    frame_refresh();
}

auto Window::is_popup_window() -> bool { return get_style() & WS_POPUPWINDOW; }

auto Window::set_popup() -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
    frame_refresh();
}

auto Window::is_popup() -> bool { return get_style() & WS_POPUP; }

auto Window::set_child() -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, WS_CHILD | WS_CLIPSIBLINGS);
    frame_refresh();
}

auto Window::is_child() -> bool { return get_style() & WS_CHILD; }

auto Window::center() -> void {
    if (positions.work.width > positions.window.width
        && positions.work.height > positions.window.height) {
        auto x { static_cast<int>((positions.work.width - positions.window.width) / 2) };
        auto y { static_cast<int>((positions.work.height - positions.window.height) / 2) };

        ::SetWindowPos(hwnd.get(), nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
    }
}

auto Window::top() -> void {
    ::SetWindowPos(hwnd.get(), HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

auto Window::bottom() -> void {
    ::SetWindowPos(hwnd.get(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

auto Window::enable_topmost() -> void {
    ::SetWindowPos(hwnd.get(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

auto Window::disable_topmost() -> void {
    ::SetWindowPos(
        hwnd.get(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

auto Window::enable_border() -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, get_style() | WS_BORDER);
    frame_refresh();
}

auto Window::disable_border() -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, get_style() & ~WS_BORDER);
    frame_refresh();
}

auto Window::cloak() -> void {
    auto cloak { TRUE };
    ::DwmSetWindowAttribute(hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_CLOAK, &cloak, sizeof(cloak));
}

auto Window::uncloak() -> void {
    auto cloak { FALSE };
    ::DwmSetWindowAttribute(hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_CLOAK, &cloak, sizeof(cloak));
}

auto Window::is_cloaked() -> bool {
    auto cloaked { DWM_CLOAKED_APP };
    ::DwmGetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_CLOAKED, &cloaked, sizeof(cloaked));

    switch (cloaked) {
        case DWM_CLOAKED_APP:
            return true;
        case DWM_CLOAKED_INHERITED:
            return true;
        case DWM_CLOAKED_SHELL:
            return true;
    }

    return false;
}

auto Window::enable_dark_mode() -> void {
    auto attribute { TRUE };
    ::DwmSetWindowAttribute(hwnd.get(),
                            ::DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                            &attribute,
                            sizeof(attribute));
}

auto Window::disable_dark_mode() -> void {
    auto attribute { FALSE };
    ::DwmSetWindowAttribute(hwnd.get(),
                            ::DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                            &attribute,
                            sizeof(attribute));
}

auto Window::set_backdrop(::DWM_SYSTEMBACKDROP_TYPE backdrop) -> void {
    ::MARGINS margins { .cxLeftWidth { 0 },
                        .cxRightWidth { 0 },
                        .cyTopHeight { 0 },
                        .cyBottomHeight { ::GetSystemMetrics(SM_CYVIRTUALSCREEN) } };

    ::DwmExtendFrameIntoClientArea(hwnd.get(), &margins);
    ::DwmSetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop));
}

auto Window::set_round_corners(::DWM_WINDOW_CORNER_PREFERENCE corner) -> void {
    ::DwmSetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
}

auto Window::focus() -> void { ::SetFocus(hwnd.get()); }

auto Window::is_focused() -> bool { return ::GetFocus() == hwnd.get(); }

auto Window::foreground() -> void { ::SetForegroundWindow(hwnd.get()); }

auto Window::is_foreground() -> bool { return ::GetForegroundWindow() == hwnd.get(); }

auto Window::active() -> void { ::SetActiveWindow(hwnd.get()); }

auto Window::is_active() -> bool { return ::GetActiveWindow() == hwnd.get(); }

auto Window::set_parent(::HWND parent) -> void {
    parent ? set_child() : set_popup();

    ::SetParent(hwnd.get(), parent);
}

auto Window::get_parent() -> ::HWND { return ::GetParent(hwnd.get()); }

auto Window::bring_to_top() -> void { ::BringWindowToTop(hwnd.get()); }

auto Window::is_topmost() -> bool { return get_ex_style() & WS_EX_TOPMOST; }

auto Window::is_visible() -> bool { return ::IsWindowVisible(hwnd.get()); }

auto Window::is_maximized() -> bool { return ::IsZoomed(hwnd.get()); }

auto Window::set_title(const std::string& title) -> void {
    ::SetWindowTextW(hwnd.get(), glow::text::to_wstring(title).c_str());
}

auto Window::set_title(const std::wstring& title) -> void {
    ::SetWindowTextW(hwnd.get(), title.c_str());
}

auto Window::set_position(const Position& position) -> void {
    ::SetWindowPos(hwnd.get(),
                   nullptr,
                   position.x,
                   position.y,
                   position.width,
                   position.height,
                   SWP_NOZORDER | SWP_NOACTIVATE);
}

auto Window::min_size(int x, int y) -> void {
    minTrackSize.x = x;
    minTrackSize.y = y;
}

auto Window::max_size(int x, int y) -> void {
    maxTrackSize.x = x;
    maxTrackSize.y = y;
}

auto Window::set_window_placement() -> void {
    windowPlacement.length = sizeof(::WINDOWPLACEMENT);
    ::SetWindowPlacement(hwnd.get(), &windowPlacement);
}

auto Window::get_window_placement() -> void {
    windowPlacement.length = sizeof(::WINDOWPLACEMENT);
    ::GetWindowPlacement(hwnd.get(), &windowPlacement);
}

auto Window::get_monitor_info() -> void {
    monitorInfo.cbSize = sizeof(::MONITORINFO);
    ::GetMonitorInfoW(::MonitorFromWindow(hwnd.get(), MONITOR_DEFAULTTONEAREST), &monitorInfo);
}

auto Window::set_style(::LONG_PTR style) -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, style);
    frame_refresh();
}

auto Window::get_style() -> ::LONG_PTR { return ::GetWindowLongPtrW(hwnd.get(), GWL_STYLE); }

auto Window::set_ex_style(::LONG_PTR exStyle) -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_EXSTYLE, exStyle);
    frame_refresh();
}

auto Window::get_ex_style() -> ::LONG_PTR { return ::GetWindowLongPtrW(hwnd.get(), GWL_EXSTYLE); }

auto Window::get_id() -> ::LONG_PTR { return ::GetWindowLongPtrW(hwnd.get(), GWLP_ID); }

auto Window::set_small_icon(::HICON hicon) -> void {
    messages.send(hwnd.get(), WM_SETICON, ICON_SMALL, hicon);
}

auto Window::set_big_icon(::HICON hicon) -> void {
    messages.send(hwnd.get(), WM_SETICON, ICON_BIG, hicon);
}

auto Window::flash() -> void {
    ::FLASHWINFO fwi { .cbSize { sizeof(::FLASHWINFO) },
                       .hwnd { hwnd.get() },
                       .dwFlags { FLASHW_CAPTION },
                       .uCount { 1 },
                       .dwTimeout { 100 } };
    ::FlashWindowEx(&fwi);
}

auto Window::start_timer(::UINT_PTR timerId, ::UINT intervalMs) -> bool {
    return ::SetTimer(hwnd.get(), timerId, intervalMs, nullptr) != 0 ? true : false;
}

auto Window::stop_timer(::UINT_PTR timerId) -> bool { return ::KillTimer(hwnd.get(), timerId); }

auto Window::enable_fullscreen() -> bool {
    if (get_style() & WS_OVERLAPPEDWINDOW) {
        positions.restoreFullscreen = positions.window;
        ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, get_style() & ~WS_OVERLAPPEDWINDOW);
        ::SetWindowPos(hwnd.get(),
                       HWND_TOP,
                       positions.monitor.x,
                       positions.monitor.y,
                       positions.monitor.width,
                       positions.monitor.height,
                       SWP_FRAMECHANGED);
    }

    return true;
}

auto Window::disable_fullscreen() -> bool {
    if (!(get_style() & WS_OVERLAPPEDWINDOW)) {
        ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, get_style() | WS_OVERLAPPEDWINDOW);
        ::SetWindowPos(hwnd.get(),
                       HWND_TOP,
                       positions.restoreFullscreen.x,
                       positions.restoreFullscreen.y,
                       positions.restoreFullscreen.width,
                       positions.restoreFullscreen.height,
                       SWP_FRAMECHANGED);
    }

    return false;
}

auto Window::client_position() -> Position {
    ::RECT rect;
    ::GetClientRect(hwnd.get(), &rect);

    return Position(rect);
}

auto Window::window_position() -> Position {
    ::RECT rect;
    ::GetWindowRect(hwnd.get(), &rect);

    return Position(rect);
}

auto Window::device_context() -> ::HDC { return ::GetDC(hwnd.get()); }

auto Window::notify_app(glow::message::Code code, std::string_view message, ::HWND receiverHwnd)
    -> void {
    messages.notify(code, message, hwnd.get(), id, receiverHwnd);
}

auto Overlapped::create(bool show) -> void {
    if (::GetClassInfoExW(glow::system::instance(), windowClass.lpszClassName, &windowClass) == 0) {
        ::RegisterClassExW(&windowClass);
    }

    ::CreateWindowExW(0,
                      windowClass.lpszClassName,
                      L"",
                      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      nullptr,
                      nullptr,
                      glow::system::instance(),
                      this);

    if (show) {
        activate();
    }
}

auto Child::create(::HWND parent, bool show) -> void {
    if (::GetClassInfoExW(glow::system::instance(), windowClass.lpszClassName, &windowClass) == 0) {
        ::RegisterClassExW(&windowClass);
    }

    ::CreateWindowExW(0,
                      windowClass.lpszClassName,
                      L"",
                      WS_CHILDWINDOW | WS_CLIPSIBLINGS,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      parent,
                      reinterpret_cast<::HMENU>(id),
                      glow::system::instance(),
                      this);

    if (show) {
        activate();
    }
}

auto EventToken::operator()(const std::string& key) -> ::EventRegistrationToken* {
    return &eventRegistrationTokens.try_emplace(key, ::EventRegistrationToken()).first->second;
}

auto WebView::create(Callback callback, bool show) -> void {
    if (::GetClassInfoExW(glow::system::instance(), windowClass.lpszClassName, &windowClass) == 0) {
        ::RegisterClassExW(&windowClass);
    }

    ::CreateWindowExW(0,
                      windowClass.lpszClassName,
                      L"",
                      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      nullptr,
                      nullptr,
                      glow::system::instance(),
                      this);

    if (show) {
        activate();
    }

    create_webview(std::move(callback));
}

auto WebView::create(::HWND parent, Callback callback, bool show) -> void {
    if (::GetClassInfoExW(glow::system::instance(), windowClass.lpszClassName, &windowClass) == 0) {
        ::RegisterClassExW(&windowClass);
    }

    ::CreateWindowExW(0,
                      windowClass.lpszClassName,
                      L"",
                      WS_CHILDWINDOW | WS_CLIPSIBLINGS,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      parent,
                      reinterpret_cast<::HMENU>(id),
                      glow::system::instance(),
                      this);

    if (show) {
        activate();
    }

    create_webview(std::move(callback));
}

auto WebView::create_webview(Callback&& callback) -> void {
    auto coInit { glow::system::co_initialize() };

    derivedMessages.on(WM_WINDOWPOSCHANGED, [this](glow::message::wm::WINDOWPOSCHANGED msg) {
        put_bounds(client_position());

        if (msg.windowPos()->flags & SWP_SHOWWINDOW) {
            show_controller();
        }

        if (msg.windowPos()->flags & SWP_HIDEWINDOW) {
            hide_controller();
        }

        return 0;
    });

    wil::com_ptr<ICoreWebView2EnvironmentOptions> createdEnvironmentOptions {
        Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>()
    };
    wil::com_ptr<ICoreWebView2EnvironmentOptions2> createdEnvironmentOptions2;
    wil::com_ptr<ICoreWebView2EnvironmentOptions3> createdEnvironmentOptions3;
    wil::com_ptr<ICoreWebView2EnvironmentOptions4> createdEnvironmentOptions4;
    wil::com_ptr<ICoreWebView2EnvironmentOptions5> createdEnvironmentOptions5;
    wil::com_ptr<ICoreWebView2EnvironmentOptions6> createdEnvironmentOptions6;
    wil::com_ptr<ICoreWebView2EnvironmentOptions7> createdEnvironmentOptions7;
    wil::com_ptr<ICoreWebView2EnvironmentOptions8> createdEnvironmentOptions8;

    createdEnvironmentOptions2
        = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions2>();
    createdEnvironmentOptions3
        = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions3>();
    createdEnvironmentOptions4
        = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions4>();
    createdEnvironmentOptions5
        = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions5>();
    createdEnvironmentOptions6
        = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions6>();
    createdEnvironmentOptions7
        = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions7>();
    createdEnvironmentOptions8
        = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions8>();

    if (createdEnvironmentOptions) {
        if (!config.environmentOptions.AdditionalBrowserArguments.empty()) {
            createdEnvironmentOptions->put_AdditionalBrowserArguments(
                glow::text::to_wstring(config.environmentOptions.AdditionalBrowserArguments)
                    .c_str());
        }

        createdEnvironmentOptions->put_AllowSingleSignOnUsingOSPrimaryAccount(
            config.environmentOptions.AllowSingleSignOnUsingOSPrimaryAccount);

        if (!config.environmentOptions.Language.empty()) {
            createdEnvironmentOptions->put_Language(
                glow::text::to_wstring(config.environmentOptions.Language).c_str());
        }

        if (!config.environmentOptions.TargetCompatibleBrowserVersion.empty()) {
            createdEnvironmentOptions->put_TargetCompatibleBrowserVersion(
                glow::text::to_wstring(config.environmentOptions.TargetCompatibleBrowserVersion)
                    .c_str());
        }
    }

    if (createdEnvironmentOptions2) {
        createdEnvironmentOptions2->put_ExclusiveUserDataFolderAccess(
            config.environmentOptions.ExclusiveUserDataFolderAccess);
    }

    if (createdEnvironmentOptions3) {
        createdEnvironmentOptions3->put_IsCustomCrashReportingEnabled(
            config.environmentOptions.IsCustomCrashReportingEnabled);
    }

    // if (createdEnvironmentOptions4) {
    //     createdEnvironmentOptions4->SetCustomSchemeRegistrations();
    // }

    if (createdEnvironmentOptions5) {
        createdEnvironmentOptions5->put_EnableTrackingPrevention(
            config.environmentOptions.EnableTrackingPrevention);
    }

    if (createdEnvironmentOptions6) {
        createdEnvironmentOptions6->put_AreBrowserExtensionsEnabled(
            config.environmentOptions.AreBrowserExtensionsEnabled);
    }

    if (createdEnvironmentOptions7) {
        createdEnvironmentOptions7->put_ChannelSearchKind(
            config.environmentOptions.ChannelSearchKind);
    }

    if (createdEnvironmentOptions8) {
        createdEnvironmentOptions8->put_ScrollBarStyle(config.environmentOptions.ScrollBarStyle);
    }

    auto environmentResult { ::CreateCoreWebView2EnvironmentWithOptions(
        config.browserExecutableFolder.c_str(),
        config.userDataFolder.c_str(),
        createdEnvironmentOptions.get(),
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this, callback { std::move(callback) }](
                ::HRESULT errorCode, ICoreWebView2Environment* createdEnvironment) -> ::HRESULT {
        if (createdEnvironment) {
            environment = wil::com_ptr<ICoreWebView2Environment>(createdEnvironment)
                              .try_query<ICoreWebView2Environment13>();
        } else {
            glow::log::log(glow::log::format_message<std::string>(errorCode));
        }

        if (environment) {
            auto controllerResult { environment->CreateCoreWebView2Controller(
                hwnd.get(),
                wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [this, callback { std::move(callback) }](
                        ::HRESULT errorCode,
                        ICoreWebView2Controller* createdController) -> ::HRESULT {
                if (createdController) {
                    controller = wil::com_ptr<ICoreWebView2Controller>(createdController)
                                     .try_query<ICoreWebView2Controller4>();
                } else {
                    glow::log::log(glow::log::format_message<std::string>(errorCode));
                }

                if (controller) {
                    controller->put_DefaultBackgroundColor({ 0, 0, 0, 0 });
                    put_bounds(client_position());

                    wil::com_ptr<ICoreWebView2> createdCore;
                    controller->get_CoreWebView2(createdCore.put());

                    if (createdCore) {
                        core = createdCore.try_query<ICoreWebView2_22>();
                    }

                    if (core) {
                        wil::com_ptr<ICoreWebView2Settings> createdSettings;
                        core->get_Settings(createdSettings.put());
                        if (createdSettings) {
                            settings = createdSettings.try_query<ICoreWebView2Settings9>();

                            if (settings) {
                                settings->put_AreBrowserAcceleratorKeysEnabled(
                                    config.settings.AreBrowserAcceleratorKeysEnabled);
                                settings->put_AreDefaultContextMenusEnabled(
                                    config.settings.AreDefaultContextMenusEnabled);
                                settings->put_AreDefaultScriptDialogsEnabled(
                                    config.settings.AreDefaultScriptDialogsEnabled);
                                settings->put_AreDevToolsEnabled(
                                    config.settings.AreDevToolsEnabled);
                                settings->put_AreHostObjectsAllowed(
                                    config.settings.AreHostObjectsAllowed);
                                settings->put_HiddenPdfToolbarItems(
                                    config.settings.HiddenPdfToolbarItems);
                                settings->put_IsBuiltInErrorPageEnabled(
                                    config.settings.IsBuiltInErrorPageEnabled);
                                settings->put_IsGeneralAutofillEnabled(
                                    config.settings.IsGeneralAutofillEnabled);
                                settings->put_IsNonClientRegionSupportEnabled(
                                    config.settings.IsNonClientRegionSupportEnabled);
                                settings->put_IsPasswordAutosaveEnabled(
                                    config.settings.IsPasswordAutosaveEnabled);
                                settings->put_IsPinchZoomEnabled(
                                    config.settings.IsPinchZoomEnabled);
                                settings->put_IsReputationCheckingRequired(
                                    config.settings.IsReputationCheckingRequired);
                                settings->put_IsScriptEnabled(config.settings.IsScriptEnabled);
                                settings->put_IsStatusBarEnabled(
                                    config.settings.IsStatusBarEnabled);
                                settings->put_IsSwipeNavigationEnabled(
                                    config.settings.IsSwipeNavigationEnabled);
                                settings->put_IsWebMessageEnabled(
                                    config.settings.IsWebMessageEnabled);
                                settings->put_IsZoomControlEnabled(
                                    config.settings.IsZoomControlEnabled);

                                if (callback) {
                                    callback();
                                } else {
                                    navigate(config.homePage);
                                }
                            }
                        }
                    }
                }

                return S_OK;
            }).Get()) };

            if (controllerResult != S_OK) {
                glow::log::log("CreateCoreWebView2Controller: {}",
                               glow::log::format_message<std::string>(controllerResult));
            }
        }

        return S_OK;
    }).Get()) };

    if (environmentResult != S_OK) {
        glow::log::log("CreateCoreWebView2EnvironmentWithOptions: {}",
                       glow::log::format_message<std::string>(environmentResult));
    }
}

auto WebView::put_bounds(const Position& position) -> void {
    if (controller) {
        controller->put_Bounds(position.rect());
    }
}

auto WebView::show_controller() -> void {
    if (controller) {
        controller->put_IsVisible(true);
    }
}

auto WebView::hide_controller() -> void {
    if (controller) {
        controller->put_IsVisible(false);
    }
}

auto WebView::get_document_title() -> std::string {
    if (core) {
        wil::unique_cotaskmem_string buffer;
        core->get_DocumentTitle(&buffer);
        return glow::text::to_string(buffer.get());
    } else {
        return {};
    }
}

auto WebView::navigate(const std::string& url) -> void {
    if (core) {
        core->Navigate(glow::text::to_wstring(url).c_str());
    }
}

auto WebView::navigate(const std::wstring& url) -> void {
    if (core) {
        core->Navigate(url.c_str());
    }
}

auto WebView::navigate_to_string(const std::string& url) -> void {
    if (core) {
        core->NavigateToString(glow::text::to_wstring(url).c_str());
    }
}

auto WebView::navigate_to_string(const std::wstring& url) -> void {
    if (core) {
        core->NavigateToString(url.c_str());
    }
}

auto WebView::virtual_host_name_mapping(const std::string& hostName,
                                        const std::filesystem::path& folder,
                                        COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND accessKind) -> void {
    if (core) {
        core->SetVirtualHostNameToFolderMapping(
            glow::text::to_wstring(hostName).c_str(), folder.c_str(), accessKind);
    }
}

auto WebView::virtual_host_name_mapping(const std::wstring& hostName,
                                        const std::filesystem::path& folder,
                                        COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND accessKind) -> void {
    if (core) {
        core->SetVirtualHostNameToFolderMapping(hostName.c_str(), folder.c_str(), accessKind);
    }
}

auto WebView::suspend(Callback callback) -> void {
    if (controller) {
        ::BOOL isVisible { false };
        controller->get_IsVisible(&isVisible);

        if (!isVisible) {
            if (core) {
                auto result { core->TrySuspend(event.make<ICoreWebView2TrySuspendCompletedHandler>(
                    [callback { std::move(callback) }](::HRESULT errorCode [[maybe_unused]],
                                                       BOOL result [[maybe_unused]]) {
                    if (callback) {
                        callback();
                    }

                    return S_OK;
                })) };

                if (result != S_OK) {
                    glow::log::log(glow::log::format_message<std::string>(result));
                }
            }
        }
    }
}

auto CALLBACK Message::procedure(::HWND hwnd, ::UINT msg, ::WPARAM wparam, ::LPARAM lparam)
    -> ::LRESULT {
    if (msg == WM_NCCREATE) {
        auto create { reinterpret_cast<::CREATESTRUCTW*>(lparam) };

        if (auto self { static_cast<Message*>(create->lpCreateParams) }; self) {
            ::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<::LONG_PTR>(self));
            self->hwnd.reset(hwnd);
        }
    }

    if (msg == WM_NCDESTROY) {
        ::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<::LONG_PTR>(nullptr));
    }

    if (auto self { reinterpret_cast<Message*>(::GetWindowLongPtrW(hwnd, 0)) }; self) {
        if (self->messages.contains(msg)) {
            return self->messages.invoke({ hwnd, msg, wparam, lparam });
        }
    }

    return glow::message::default_procedure({ hwnd, msg, wparam, lparam });
}

auto Message::create() -> void {
    if (::GetClassInfoExW(glow::system::instance(), windowClass.lpszClassName, &windowClass) == 0) {
        ::RegisterClassExW(&windowClass);
    }

    ::CreateWindowExW(0,
                      windowClass.lpszClassName,
                      L"",
                      0,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      HWND_MESSAGE,
                      nullptr,
                      glow::system::instance(),
                      this);
}

auto Message::operator()() -> int { return glow::message::run_loop(); }

auto Message::notify_app(glow::message::Code code, std::string_view message, ::HWND receiverHwnd)
    -> void {
    messages.notify(code, message, hwnd.get(), id, receiverHwnd);
}
} // namespace glow::window
