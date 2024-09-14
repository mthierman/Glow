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
Window::Window() {
    baseMessages.on(WM_CREATE, [this](glow::message::wm::CREATE /* msg */) {
        brushes.custom.reset(backgroundColor.brush());
        refresh_dpi();

        return 0;
    });

    baseMessages.on(WM_SETTINGCHANGE, [this](glow::message::wm::MSG /* msg */) {
        brushes.system.reset(glow::color::Color(winrt::UIColorType::Background).brush());
        invalidate_rect();

        return 0;
    });

    baseMessages.on(WM_WINDOWPOSCHANGED, [this](glow::message::wm::WINDOWPOSCHANGED msg) {
        auto windowPos { msg.windowPos() };

        positions.window = to_position(windowPos);
        positions.client = to_position(client_rect());

        get_window_placement();

        get_monitor_info();
        positions.monitor = to_position(monitorInfo.rcMonitor);
        positions.work = to_position(monitorInfo.rcWork);

        return 0;
    });

    baseMessages.on(WM_DPICHANGED, [this](glow::message::wm::DPICHANGED msg) {
        refresh_dpi();
        set_position(msg.suggestedRect());

        return 0;
    });

    defaultMessages.on(WM_ERASEBKGND, [this](glow::message::wm::ERASEBKGND msg) {
        return erase_background(msg.deviceContext());
    });

    defaultMessages.on(WM_CLOSE, [this](glow::message::wm::MSG /* msg */) {
        hwnd.reset();

        return 0;
    });
}

auto CALLBACK Window::procedure(::HWND hwnd,
                                ::UINT msg,
                                ::WPARAM wparam,
                                ::LPARAM lparam) -> ::LRESULT {
    if (msg == WM_NCCREATE) {
        auto create { reinterpret_cast<::CREATESTRUCTW*>(lparam) };

        if (auto self { static_cast<Window*>(create->lpCreateParams) }; self) {
            ::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<::LONG_PTR>(self));
            self->hwnd.reset(hwnd);
        }
    }

    if (msg == WM_NCDESTROY) {
        ::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<::LONG_PTR>(nullptr));
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

auto Window::set_background_style(BackgroundStyle style) -> void {
    backgroundStyle = style;
    invalidate_rect();
}

auto Window::set_background_color(glow::color::Color color) -> void {
    backgroundColor = color;
    brushes.custom.reset(backgroundColor.brush());
    invalidate_rect();
}

auto Window::erase_background(::HDC hdc) -> int {
    switch (backgroundStyle) {
        case BackgroundStyle::Transparent: {
            paint_background(hdc, brushes.transparent);
        } break;
        case BackgroundStyle::System: {
            paint_background(hdc, brushes.system);
        } break;
        case BackgroundStyle::Black: {
            paint_background(hdc, brushes.black);
        } break;
        case BackgroundStyle::White: {
            paint_background(hdc, brushes.white);
        } break;
        case BackgroundStyle::Custom: {
            paint_background(hdc, brushes.custom);
        } break;
    }

    return 1;
}

auto Window::paint_background(::HDC hdc, const wil::unique_hbrush& brush) -> void {
    auto rect { client_rect() };
    ::FillRect(hdc, &rect, brush.get());
}

auto Window::register_class(::WNDCLASSEXW& windowClass) -> void {
    if (::GetClassInfoExW(glow::system::instance(), windowClass.lpszClassName, &windowClass) == 0) {
        ::RegisterClassExW(&windowClass);
    }
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

auto Window::set_position(::RECT rect) -> void {
    auto position { to_position(rect) };
    ::SetWindowPos(hwnd.get(),
                   nullptr,
                   position.x,
                   position.y,
                   position.width,
                   position.height,
                   SWP_NOZORDER | SWP_NOACTIVATE);
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

auto Overlapped::create(bool show) -> void {
    register_class(windowClass);
    ::CreateWindowExW(0,
                      windowClass.lpszClassName,
                      L"",
                      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | (show ? WS_VISIBLE : 0),
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      nullptr,
                      nullptr,
                      glow::system::instance(),
                      this);
}

auto Child::create(::HWND parent, bool show) -> void {
    register_class(windowClass);
    ::CreateWindowExW(0,
                      windowClass.lpszClassName,
                      L"",
                      WS_CHILDWINDOW | WS_CLIPSIBLINGS | (show ? WS_VISIBLE : 0),
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      parent,
                      reinterpret_cast<::HMENU>(id),
                      glow::system::instance(),
                      this);
}

auto WebView::create(Callback callback, bool show) -> void {
    register_class(windowClass);
    ::CreateWindowExW(0,
                      windowClass.lpszClassName,
                      L"",
                      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | (show ? WS_VISIBLE : 0),
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      nullptr,
                      nullptr,
                      glow::system::instance(),
                      this);
    // set_background(Background::Custom);
    // set_background_color(
    //     config.backgroundColor.R, config.backgroundColor.G, config.backgroundColor.B);
    create_webview(callback);
}

auto WebView::create(::HWND parent, Callback callback, bool show) -> void {
    register_class(windowClass);
    ::CreateWindowExW(0,
                      windowClass.lpszClassName,
                      L"",
                      WS_CHILDWINDOW | WS_CLIPSIBLINGS | (show ? WS_VISIBLE : 0),
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      parent,
                      reinterpret_cast<::HMENU>(id),
                      glow::system::instance(),
                      this);
    // set_background(Background::Custom);
    // set_background_color(
    //     config.backgroundColor.R, config.backgroundColor.G, config.backgroundColor.B);
    create_webview(callback);
}

auto WebView::create_webview(Callback callback) -> void {
    auto coInit { glow::system::co_initialize() };

    derivedMessages.on(WM_WINDOWPOSCHANGED, [this](glow::message::wm::WINDOWPOSCHANGED msg) {
        put_bounds(client_rect());

        if (msg.windowPos().flags & SWP_SHOWWINDOW) {
            show_controller();
        }

        if (msg.windowPos().flags & SWP_HIDEWINDOW) {
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
        config.browserExecutableFolder.c_str(),
        config.userDataFolder.c_str(),
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
            // controller->put_DefaultBackgroundColor(config.backgroundColor);

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

            core->add_DOMContentLoaded(
                handler<ICoreWebView2DOMContentLoadedEventHandler>(
                    [this](ICoreWebView2* /* sender */,
                           ICoreWebView2DOMContentLoadedEventArgs* /* args */) -> ::HRESULT {
                activate();
                core->remove_DOMContentLoaded(tokens("DOMContentLoaded"));

                return S_OK;
            }).Get(),
                &tokens("DOMContentLoaded"));

            if (callback) {
                callback();
            } else {
                navigate(config.homePage);
            }

            return S_OK;
        }).Get());

        return S_OK;
    }).Get());
}

auto WebView::put_bounds(const Position& position) -> void {
    if (controller) {
        controller->put_Bounds(glow::window::to_rect(position));
    }
}

auto WebView::put_bounds(const ::RECT& rect) -> void {
    if (controller) {
        controller->put_Bounds(rect);
    }
}

auto WebView::put_bounds(const ::SIZE& size) -> void {
    if (controller) {
        controller->put_Bounds(glow::window::to_rect(size));
    }
}

auto WebView::put_bounds(const ::WINDOWPOS& windowPos) -> void {
    if (controller) {
        controller->put_Bounds(glow::window::to_rect(windowPos));
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

auto WebView::Tokens::operator()(const std::string& key) -> ::EventRegistrationToken& {
    return tokens.try_emplace(key, ::EventRegistrationToken()).first->second;
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
