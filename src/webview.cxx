// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/log.hxx>
#include <glow/text.hxx>
#include <glow/webview.hxx>
#include <glow/window.hxx>

namespace glow::webview {
auto WebViewEnvironment::create(std::function<void()> callback) -> ::HRESULT {
    auto browserExecutableFolder { glow::text::utf8_to_utf16(m_browserExecutableFolder.string()) };
    auto userDataFolder { glow::text::utf8_to_utf16(m_userDataFolder.string()) };

    wil::com_ptr<ICoreWebView2EnvironmentOptions> environmentOptions {
        Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>()
    };
    wil::com_ptr<ICoreWebView2EnvironmentOptions2> environmentOptions2;
    wil::com_ptr<ICoreWebView2EnvironmentOptions3> environmentOptions3;
    wil::com_ptr<ICoreWebView2EnvironmentOptions4> environmentOptions4;
    wil::com_ptr<ICoreWebView2EnvironmentOptions5> environmentOptions5;
    wil::com_ptr<ICoreWebView2EnvironmentOptions6> environmentOptions6;
    wil::com_ptr<ICoreWebView2EnvironmentOptions7> environmentOptions7;
    wil::com_ptr<ICoreWebView2EnvironmentOptions8> environmentOptions8;

    if (!m_webViewEnvironmentOptions.AdditionalBrowserArguments.empty()) {
        environmentOptions->put_AdditionalBrowserArguments(
            glow::text::utf8_to_utf16(m_webViewEnvironmentOptions.AdditionalBrowserArguments)
                .c_str());
    }

    environmentOptions->put_AllowSingleSignOnUsingOSPrimaryAccount(
        m_webViewEnvironmentOptions.AllowSingleSignOnUsingOSPrimaryAccount);

    if (!m_webViewEnvironmentOptions.Language.empty()) {
        environmentOptions->put_Language(
            glow::text::utf8_to_utf16(m_webViewEnvironmentOptions.Language).c_str());
    }

    if (!m_webViewEnvironmentOptions.TargetCompatibleBrowserVersion.empty()) {
        environmentOptions->put_TargetCompatibleBrowserVersion(
            glow::text::utf8_to_utf16(m_webViewEnvironmentOptions.TargetCompatibleBrowserVersion)
                .c_str());
    }

    environmentOptions2 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions2>();

    environmentOptions2->put_ExclusiveUserDataFolderAccess(
        m_webViewEnvironmentOptions.ExclusiveUserDataFolderAccess);

    environmentOptions3 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions3>();

    environmentOptions3->put_IsCustomCrashReportingEnabled(
        m_webViewEnvironmentOptions.IsCustomCrashReportingEnabled);

    // environmentOptions4 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions4>();

    // environmentOptions4->SetCustomSchemeRegistrations();

    environmentOptions5 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions5>();

    environmentOptions5->put_EnableTrackingPrevention(
        m_webViewEnvironmentOptions.EnableTrackingPrevention);

    environmentOptions6 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions6>();

    environmentOptions6->put_AreBrowserExtensionsEnabled(
        m_webViewEnvironmentOptions.AreBrowserExtensionsEnabled);

    environmentOptions7 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions7>();

    environmentOptions7->put_ChannelSearchKind(m_webViewEnvironmentOptions.ChannelSearchKind);

    environmentOptions8 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions8>();

    environmentOptions8->put_ScrollBarStyle(COREWEBVIEW2_SCROLLBAR_STYLE_FLUENT_OVERLAY);

    return CreateCoreWebView2EnvironmentWithOptions(
        m_browserExecutableFolder.c_str(),
        userDataFolder.c_str(),
        environmentOptions.get(),
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [=, this](::HRESULT /* errorCode */,
                      ICoreWebView2Environment* createdEnvironment) -> ::HRESULT {
        auto environment { wil::com_ptr<ICoreWebView2Environment>(createdEnvironment) };
        m_environment = environment.try_query<ICoreWebView2Environment13>();

        callback();

        return S_OK;
    }).Get());
}

auto WebViewEnvironment::close() -> void { m_environment.reset(); }

auto WebView::create(const WebViewEnvironment& webViewEnvironment,
                     ::HWND hwnd,
                     std::function<void()> callback) -> ::HRESULT {
    return webViewEnvironment.m_environment.get()->CreateCoreWebView2Controller(
        hwnd,
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
            [=, this](::HRESULT /* errorCode */,
                      ICoreWebView2Controller* createdController) -> ::HRESULT {
        auto controller { wil::com_ptr<ICoreWebView2Controller>(createdController) };
        glow::log::log("{}", glow::log::get_last_error());
        m_controller = controller.try_query<ICoreWebView2Controller4>();
        m_controller->put_DefaultBackgroundColor(COREWEBVIEW2_COLOR { 0, 0, 0, 0 });

        wil::com_ptr<ICoreWebView2> core;
        m_controller->get_CoreWebView2(core.put());
        m_core = core.try_query<ICoreWebView2_22>();

        wil::com_ptr<ICoreWebView2Settings> settings;
        m_core->get_Settings(settings.put());
        m_settings = settings.try_query<ICoreWebView2Settings9>();

        m_settings->put_AreBrowserAcceleratorKeysEnabled(
            m_webViewSettings.AreBrowserAcceleratorKeysEnabled);
        m_settings->put_AreDefaultContextMenusEnabled(
            m_webViewSettings.AreDefaultContextMenusEnabled);
        m_settings->put_AreDefaultScriptDialogsEnabled(
            m_webViewSettings.AreDefaultScriptDialogsEnabled);
        m_settings->put_AreDevToolsEnabled(m_webViewSettings.AreDevToolsEnabled);
        m_settings->put_AreHostObjectsAllowed(m_webViewSettings.AreHostObjectsAllowed);
        m_settings->put_HiddenPdfToolbarItems(m_webViewSettings.HiddenPdfToolbarItems);
        m_settings->put_IsBuiltInErrorPageEnabled(m_webViewSettings.IsBuiltInErrorPageEnabled);
        m_settings->put_IsGeneralAutofillEnabled(m_webViewSettings.IsGeneralAutofillEnabled);
        m_settings->put_IsNonClientRegionSupportEnabled(
            m_webViewSettings.IsNonClientRegionSupportEnabled);
        m_settings->put_IsPasswordAutosaveEnabled(m_webViewSettings.IsPasswordAutosaveEnabled);
        m_settings->put_IsPinchZoomEnabled(m_webViewSettings.IsPinchZoomEnabled);
        m_settings->put_IsReputationCheckingRequired(
            m_webViewSettings.IsReputationCheckingRequired);
        m_settings->put_IsScriptEnabled(m_webViewSettings.IsScriptEnabled);
        m_settings->put_IsStatusBarEnabled(m_webViewSettings.IsStatusBarEnabled);
        m_settings->put_IsSwipeNavigationEnabled(m_webViewSettings.IsSwipeNavigationEnabled);
        m_settings->put_IsWebMessageEnabled(m_webViewSettings.IsWebMessageEnabled);
        m_settings->put_IsZoomControlEnabled(m_webViewSettings.IsZoomControlEnabled);

        callback();

        return S_OK;
    }).Get());
}

auto WebView::close() -> void {
    m_settings.reset();
    m_core.reset();
    m_controller.reset();
}

auto WebView::put_bounds(::RECT rect) -> void {
    if (m_controller) {
        m_controller->put_Bounds(rect);
    }
}

auto WebView::put_bounds(::SIZE size) -> void {
    if (m_controller) {
        m_controller->put_Bounds(
            ::RECT { .left = 0, .top = 0, .right = size.cx, .bottom = size.cy });
    }
}

auto WebView::put_bounds(::WINDOWPOS windowPos) -> void {
    if (m_controller) {
        m_controller->put_Bounds(
            ::RECT { .left = 0, .top = 0, .right = windowPos.cx, .bottom = windowPos.cy });
    }
}

auto WebView::put_bounds(::HWND hwnd) -> void {
    if (m_controller) {
        m_controller->put_Bounds(glow::window::get_client_rect(hwnd));
    }
}

auto WebView::show() -> void {
    if (m_controller) {
        m_controller->put_IsVisible(true);
    }
}

auto WebView::hide() -> void {
    if (m_controller) {
        m_controller->put_IsVisible(false);
    }
}

auto WebView::navigate(const std::string& url) -> void {
    if (m_core) {
        m_core->Navigate(glow::text::utf8_to_utf16(url).c_str());
    }
}

auto WebView::navigate(const std::wstring& url) -> void {
    if (m_core) {
        m_core->Navigate(url.c_str());
    }
}

auto WebView::test() -> void {
    if (m_core) {
        // std::wstring title;
        wil::unique_cotaskmem_string buffer;
        m_core->get_DocumentTitle(&buffer);
        // auto test = std::wstring(buffer.get());
        glow::log::log(L"{}", std::wstring(buffer.get()));
    }
}
}; // namespace glow::webview
