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

namespace glow::window {
auto CALLBACK Window::procedure(::HWND hwnd,
                                ::UINT msg,
                                ::WPARAM wparam,
                                ::LPARAM lparam) -> ::LRESULT {
    if (msg == WM_NCCREATE) {
        auto create { reinterpret_cast<::CREATESTRUCTW*>(lparam) };

        if (auto self { static_cast<Window*>(create->lpCreateParams) }; self) {
            ::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<::LONG_PTR>(self));
            self->hwnd.reset(hwnd);
            self->refresh_dpi();
        }
    }

    if (auto self { reinterpret_cast<Window*>(::GetWindowLongPtrW(hwnd, 0)) }; self) {
        if (msg == WM_NCDESTROY) {
            ::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<::LONG_PTR>(nullptr));
        }

        if (msg == WM_SETTINGCHANGE) {
            self->brushes.system.reset(
                glow::color::create_brush(glow::color::system(winrt::UIColorType::Background)));
            self->invalidate_rect();
        }

        if (msg == WM_WINDOWPOSCHANGED) {
            auto windowPos { reinterpret_cast<::LPWINDOWPOS>(lparam) };

            self->positions.window = to_position(*windowPos);
            self->positions.client = to_position(self->client_rect());

            self->get_window_placement();

            self->get_monitor_info();
            self->positions.monitor = to_position(self->monitorInfo.rcMonitor);
            self->positions.work = to_position(self->monitorInfo.rcWork);
        }

        if (msg == WM_DPICHANGED) {
            self->refresh_dpi();
            self->set_position(to_position(*reinterpret_cast<::LPRECT>(lparam)));
        }

        if (self->messages.contains(msg)) {
            return self->messages.invoke({ hwnd, msg, wparam, lparam });
        }

        if (msg == WM_ERASEBKGND) {
            auto hdc { reinterpret_cast<::HDC>(wparam) };

            switch (self->states.background) {
                case Background::Transparent: {
                    self->paint_background(hdc, self->brushes.transparent);
                } break;
                case Background::System: {
                    self->paint_background(hdc, self->brushes.system);
                } break;
                case Background::Black: {
                    self->paint_background(hdc, self->brushes.black);
                } break;
                case Background::White: {
                    self->paint_background(hdc, self->brushes.white);
                } break;
                case Background::Custom: {
                    if (self->brushes.custom) {
                        self->paint_background(hdc, self->brushes.custom);
                    } else {
                        self->paint_background(hdc, self->brushes.system);
                    }
                } break;
            }

            return 1;
        }

        if (msg == WM_CLOSE) {
            self->close();

            return 0;
        }
    }

    return glow::message::default_procedure({ hwnd, msg, wparam, lparam });
}

auto Window::register_class(::WNDCLASSEXW& windowClass) -> void {
    if (::GetClassInfoExW(glow::system::instance(), windowClass.lpszClassName, &windowClass) == 0) {
        ::RegisterClassExW(&windowClass);
    }
}

auto Window::default_icon() -> ::HICON {
    return icons.app ? icons.app.get() : glow::system::system_icon();
}

auto Window::refresh_dpi() -> void {
    dpi = static_cast<size_t>(::GetDpiForWindow(hwnd.get()));
    scale = (static_cast<double>(dpi) / static_cast<double>(USER_DEFAULT_SCREEN_DPI));
}

auto Window::activate() -> void { ::ShowWindow(hwnd.get(), SW_NORMAL); }

auto Window::show() -> void { ::ShowWindow(hwnd.get(), SW_SHOW); }

auto Window::hide() -> void { ::ShowWindow(hwnd.get(), SW_HIDE); }

auto Window::maximize() -> void { ::ShowWindow(hwnd.get(), SW_MAXIMIZE); }

auto Window::minimize() -> void { ::ShowWindow(hwnd.get(), SW_MINIMIZE); }

auto Window::restore() -> void { ::ShowWindow(hwnd.get(), SW_RESTORE); }

auto Window::refresh_frame() -> void {
    ::SetWindowPos(hwnd.get(),
                   nullptr,
                   0,
                   0,
                   0,
                   0,
                   SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED
                       | SWP_SHOWWINDOW);
}

auto Window::set_overlapped_window() -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
    refresh_frame();
}

auto Window::is_overlapped_window() -> bool { return get_style() & WS_OVERLAPPEDWINDOW; }

auto Window::set_popup_window() -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, WS_POPUPWINDOW | WS_CLIPCHILDREN);
    refresh_frame();
}

auto Window::is_popup_window() -> bool { return get_style() & WS_POPUPWINDOW; }

auto Window::set_popup() -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
    refresh_frame();
}

auto Window::is_popup() -> bool { return get_style() & WS_POPUP; }

auto Window::set_child() -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, WS_CHILD | WS_CLIPSIBLINGS);
    refresh_frame();
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
    refresh_frame();
}

auto Window::disable_border() -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, get_style() & ~WS_BORDER);
    refresh_frame();
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

auto Window::set_caption_color(uint8_t r, uint8_t g, uint8_t b) -> void {
    auto colorref { RGB(r, g, b) };
    ::DwmSetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR, &colorref, sizeof(colorref));
}

auto Window::set_caption_color(const winrt::Color& color) -> void {
    auto colorref { glow::color::to_colorref(color) };
    ::DwmSetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR, &colorref, sizeof(colorref));
}

auto Window::set_border_color(uint8_t r, uint8_t g, uint8_t b) -> void {
    auto colorref { RGB(r, g, b) };
    ::DwmSetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, &colorref, sizeof(colorref));
}

auto Window::set_border_color(const winrt::Color& color) -> void {
    auto colorref { glow::color::to_colorref(color) };
    ::DwmSetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, &colorref, sizeof(colorref));
}

auto Window::set_text_color(uint8_t r, uint8_t g, uint8_t b) -> void {
    auto colorref { RGB(r, g, b) };
    ::DwmSetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_TEXT_COLOR, &colorref, sizeof(colorref));
}

auto Window::set_text_color(const winrt::Color& color) -> void {
    auto colorref { glow::color::to_colorref(color) };
    ::DwmSetWindowAttribute(
        hwnd.get(), ::DWMWINDOWATTRIBUTE::DWMWA_TEXT_COLOR, &colorref, sizeof(colorref));
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

auto Window::set_position(Position position) -> void {
    ::SetWindowPos(hwnd.get(),
                   nullptr,
                   position.x,
                   position.y,
                   position.width,
                   position.height,
                   SWP_NOZORDER | SWP_NOACTIVATE);
}

auto Window::set_window_placement() -> void { ::SetWindowPlacement(hwnd.get(), &windowPlacement); }

auto Window::get_window_placement() -> void { ::GetWindowPlacement(hwnd.get(), &windowPlacement); }

auto Window::get_monitor_info() -> void {
    ::GetMonitorInfoW(::MonitorFromWindow(hwnd.get(), MONITOR_DEFAULTTONEAREST), &monitorInfo);
}

auto Window::set_style(::LONG_PTR style) -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_STYLE, style);
    refresh_frame();
}

auto Window::get_style() -> ::LONG_PTR { return ::GetWindowLongPtrW(hwnd.get(), GWL_STYLE); }

auto Window::set_ex_style(::LONG_PTR exStyle) -> void {
    ::SetWindowLongPtrW(hwnd.get(), GWL_EXSTYLE, exStyle);
    refresh_frame();
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

auto Window::close() -> void { hwnd.reset(); }

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

auto Window::set_background(Background background) -> void {
    states.background = background;
    invalidate_rect();
}

auto Window::set_background_color(uint8_t r, uint8_t g, uint8_t b) -> void {
    brushes.custom.reset(glow::color::create_brush(r, g, b));
}

auto Window::set_background_color(const winrt::Color& color) -> void {
    brushes.custom.reset(glow::color::create_brush(color));
}

auto Window::paint_background(::HDC hdc, const wil::unique_hbrush& brush) -> void {
    auto rect { client_rect() };
    ::FillRect(hdc, &rect, brush.get());
}

auto Window::client_rect() -> ::RECT {
    ::RECT rect;
    ::GetClientRect(hwnd.get(), &rect);

    return rect;
}

auto Window::window_rect() -> ::RECT {
    ::RECT rect;
    ::GetWindowRect(hwnd.get(), &rect);

    return rect;
}

auto Window::invalidate_rect() -> void {
    auto rect { client_rect() };
    ::InvalidateRect(hwnd.get(), &rect, TRUE);
}

auto Window::device_context() -> ::HDC { return ::GetDC(hwnd.get()); }

auto Window::notify_app(glow::message::Code code,
                        std::string_view message,
                        ::HWND receiverHwnd) -> void {
    messages.notify(code, message, hwnd.get(), id, receiverHwnd);
}

auto Overlapped::create() -> void {
    register_class(windowClass);
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
}

auto Child::create(::HWND parent) -> void {
    register_class(windowClass);
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
}

auto WebView::create(Callback callback) -> void {
    register_class(windowClass);
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
    create_webview(callback);
}

auto WebView::create(::HWND parent, Callback callback) -> void {
    register_class(windowClass);
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
    create_webview(callback);
}

auto WebView::create_webview(Callback callback) -> void {
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

    if (!config.environmentOptions.AdditionalBrowserArguments.empty()) {
        createdEnvironmentOptions->put_AdditionalBrowserArguments(
            glow::text::to_wstring(config.environmentOptions.AdditionalBrowserArguments).c_str());
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

    createdEnvironmentOptions2->put_ExclusiveUserDataFolderAccess(
        config.environmentOptions.ExclusiveUserDataFolderAccess);
    createdEnvironmentOptions3->put_IsCustomCrashReportingEnabled(
        config.environmentOptions.IsCustomCrashReportingEnabled);
    // createdEnvironmentOptions4->SetCustomSchemeRegistrations();
    createdEnvironmentOptions5->put_EnableTrackingPrevention(
        config.environmentOptions.EnableTrackingPrevention);
    createdEnvironmentOptions6->put_AreBrowserExtensionsEnabled(
        config.environmentOptions.AreBrowserExtensionsEnabled);
    createdEnvironmentOptions7->put_ChannelSearchKind(config.environmentOptions.ChannelSearchKind);
    createdEnvironmentOptions8->put_ScrollBarStyle(config.environmentOptions.ScrollBarStyle);

    CreateCoreWebView2EnvironmentWithOptions(
        config.environmentOptions.browserExecutableFolder.c_str(),
        config.environmentOptions.userDataFolder.c_str(),
        createdEnvironmentOptions.get(),
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this, callback { std::move(callback) }](
                ::HRESULT /* errorCode */,
                ICoreWebView2Environment* createdEnvironment) -> ::HRESULT {
        environment = wil::com_ptr<ICoreWebView2Environment>(createdEnvironment)
                          .try_query<ICoreWebView2Environment13>();

        environment->CreateCoreWebView2Controller(
            hwnd.get(),
            wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                [this, callback { std::move(callback) }](
                    ::HRESULT /* errorCode */,
                    ICoreWebView2Controller* createdController) -> ::HRESULT {
            controller = wil::com_ptr<ICoreWebView2Controller>(createdController)
                             .try_query<ICoreWebView2Controller4>();
            controller->put_DefaultBackgroundColor(config.settings.backgroundColor);

            wil::com_ptr<ICoreWebView2> createdCore;
            controller->get_CoreWebView2(createdCore.put());
            core = createdCore.try_query<ICoreWebView2_22>();

            wil::com_ptr<ICoreWebView2Settings> createdSettings;
            core->get_Settings(createdSettings.put());
            settings = createdSettings.try_query<ICoreWebView2Settings9>();

            settings->put_AreBrowserAcceleratorKeysEnabled(
                config.settings.AreBrowserAcceleratorKeysEnabled);
            settings->put_AreDefaultContextMenusEnabled(
                config.settings.AreDefaultContextMenusEnabled);
            settings->put_AreDefaultScriptDialogsEnabled(
                config.settings.AreDefaultScriptDialogsEnabled);
            settings->put_AreDevToolsEnabled(config.settings.AreDevToolsEnabled);
            settings->put_AreHostObjectsAllowed(config.settings.AreHostObjectsAllowed);
            settings->put_HiddenPdfToolbarItems(config.settings.HiddenPdfToolbarItems);
            settings->put_IsBuiltInErrorPageEnabled(config.settings.IsBuiltInErrorPageEnabled);
            settings->put_IsGeneralAutofillEnabled(config.settings.IsGeneralAutofillEnabled);
            settings->put_IsNonClientRegionSupportEnabled(
                config.settings.IsNonClientRegionSupportEnabled);
            settings->put_IsPasswordAutosaveEnabled(config.settings.IsPasswordAutosaveEnabled);
            settings->put_IsPinchZoomEnabled(config.settings.IsPinchZoomEnabled);
            settings->put_IsReputationCheckingRequired(
                config.settings.IsReputationCheckingRequired);
            settings->put_IsScriptEnabled(config.settings.IsScriptEnabled);
            settings->put_IsStatusBarEnabled(config.settings.IsStatusBarEnabled);
            settings->put_IsSwipeNavigationEnabled(config.settings.IsSwipeNavigationEnabled);
            settings->put_IsWebMessageEnabled(config.settings.IsWebMessageEnabled);
            settings->put_IsZoomControlEnabled(config.settings.IsZoomControlEnabled);

            // ::RECT rect;
            // ::GetClientRect(hwnd, &rect);
            // put_bounds(rect);

            if (callback) {
                callback();
            }

            return S_OK;
        }).Get());

        return S_OK;
    }).Get());
}

auto WebView::update_bounds() -> void {
    if (controller) {
        controller->put_Bounds(client_rect());
    }
}

auto to_position(const ::RECT& rect) -> Position {
    return Position { .x { rect.left },
                      .y { rect.top },
                      .width { rect.right - rect.left },
                      .height { rect.bottom - rect.top } };
}

auto to_position(const ::WINDOWPOS& windowPos) -> Position {
    return Position {
        .x { windowPos.x }, .y { windowPos.y }, .width { windowPos.cx }, .height { windowPos.cy }
    };
}

auto to_position(const ::SIZE& size) -> Position {
    return Position { .x { 0 }, .y { 0 }, .width { size.cx }, .height { size.cy } };
}

auto to_rect(const Position& position) -> ::RECT {
    return ::RECT { .left { position.x },
                    .top { position.y },
                    .right { position.width },
                    .bottom { position.height } };
}

auto to_rect(const ::WINDOWPOS& windowPos) -> ::RECT {
    return ::RECT { .left { windowPos.x },
                    .top { windowPos.y },
                    .right { windowPos.cx },
                    .bottom { windowPos.cy } };
}

auto to_rect(const ::SIZE& size) -> ::RECT {
    return ::RECT { .left { 0 }, .top { 0 }, .right { size.cx }, .bottom { size.cy } };
}
} // namespace glow::window
