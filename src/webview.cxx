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
auto Environment::create(Callback callback) -> ::HRESULT {
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

    environmentOptions2 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions2>();
    environmentOptions3 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions3>();
    environmentOptions4 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions4>();
    environmentOptions5 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions5>();
    environmentOptions6 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions6>();
    environmentOptions7 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions7>();
    environmentOptions8 = environmentOptions.try_query<ICoreWebView2EnvironmentOptions8>();

    if (!options.AdditionalBrowserArguments.empty()) {
        environmentOptions->put_AdditionalBrowserArguments(
            glow::text::to_wstring(options.AdditionalBrowserArguments).c_str());
    }

    environmentOptions->put_AllowSingleSignOnUsingOSPrimaryAccount(
        options.AllowSingleSignOnUsingOSPrimaryAccount);

    if (!options.Language.empty()) {
        environmentOptions->put_Language(glow::text::to_wstring(options.Language).c_str());
    }

    if (!options.TargetCompatibleBrowserVersion.empty()) {
        environmentOptions->put_TargetCompatibleBrowserVersion(
            glow::text::to_wstring(options.TargetCompatibleBrowserVersion).c_str());
    }

    environmentOptions2->put_ExclusiveUserDataFolderAccess(options.ExclusiveUserDataFolderAccess);
    environmentOptions3->put_IsCustomCrashReportingEnabled(options.IsCustomCrashReportingEnabled);
    // environmentOptions4->SetCustomSchemeRegistrations();
    environmentOptions5->put_EnableTrackingPrevention(options.EnableTrackingPrevention);
    environmentOptions6->put_AreBrowserExtensionsEnabled(options.AreBrowserExtensionsEnabled);
    environmentOptions7->put_ChannelSearchKind(options.ChannelSearchKind);
    environmentOptions8->put_ScrollBarStyle(options.ScrollBarStyle);

    return CreateCoreWebView2EnvironmentWithOptions(
        options.browserExecutableFolder.c_str(),
        options.userDataFolder.c_str(),
        environmentOptions.get(),
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this, callback { std::move(callback) }](
                ::HRESULT /* errorCode */,
                ICoreWebView2Environment* createdEnvironment) -> ::HRESULT {
        environment = wil::com_ptr<ICoreWebView2Environment>(createdEnvironment)
                          .try_query<ICoreWebView2Environment13>();

        if (callback) {
            callback();
        }

        return S_OK;
    }).Get());
}

auto Environment::close() -> void { environment.reset(); }

auto WebView::create(const Environment& environment, ::HWND hwnd, Callback callback) -> ::HRESULT {
    return environment.environment.get()->CreateCoreWebView2Controller(
        hwnd,
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
            [this, callback { std::move(callback) }, hwnd](
                ::HRESULT /* errorCode */,
                ICoreWebView2Controller* createdController) -> ::HRESULT {
        controller = wil::com_ptr<ICoreWebView2Controller>(createdController)
                         .try_query<ICoreWebView2Controller4>();
        controller->put_DefaultBackgroundColor(options.backgroundColor);

        wil::com_ptr<ICoreWebView2> createdCore;
        controller->get_CoreWebView2(createdCore.put());
        core = createdCore.try_query<ICoreWebView2_22>();

        wil::com_ptr<ICoreWebView2Settings> createdSettings;
        core->get_Settings(createdSettings.put());
        settings = createdSettings.try_query<ICoreWebView2Settings9>();

        settings->put_AreBrowserAcceleratorKeysEnabled(options.AreBrowserAcceleratorKeysEnabled);
        settings->put_AreDefaultContextMenusEnabled(options.AreDefaultContextMenusEnabled);
        settings->put_AreDefaultScriptDialogsEnabled(options.AreDefaultScriptDialogsEnabled);
        settings->put_AreDevToolsEnabled(options.AreDevToolsEnabled);
        settings->put_AreHostObjectsAllowed(options.AreHostObjectsAllowed);
        settings->put_HiddenPdfToolbarItems(options.HiddenPdfToolbarItems);
        settings->put_IsBuiltInErrorPageEnabled(options.IsBuiltInErrorPageEnabled);
        settings->put_IsGeneralAutofillEnabled(options.IsGeneralAutofillEnabled);
        settings->put_IsNonClientRegionSupportEnabled(options.IsNonClientRegionSupportEnabled);
        settings->put_IsPasswordAutosaveEnabled(options.IsPasswordAutosaveEnabled);
        settings->put_IsPinchZoomEnabled(options.IsPinchZoomEnabled);
        settings->put_IsReputationCheckingRequired(options.IsReputationCheckingRequired);
        settings->put_IsScriptEnabled(options.IsScriptEnabled);
        settings->put_IsStatusBarEnabled(options.IsStatusBarEnabled);
        settings->put_IsSwipeNavigationEnabled(options.IsSwipeNavigationEnabled);
        settings->put_IsWebMessageEnabled(options.IsWebMessageEnabled);
        settings->put_IsZoomControlEnabled(options.IsZoomControlEnabled);

        ::RECT rect;
        ::GetClientRect(hwnd, &rect);
        put_bounds(rect);

        if (callback) {
            callback();
        }

        return S_OK;
    }).Get());
}

auto WebView::close() -> void {
    settings.reset();
    core.reset();
    controller.reset();
}

auto WebView::put_bounds(const glow::window::Position& position) -> void {
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

template <typename T> auto WebView::put_bounds(const T& window) -> void {
    if (controller) {
        controller->put_Bounds(window.client_rect());
    }
}

auto WebView::show() -> void {
    if (controller) {
        controller->put_IsVisible(true);
    }
}

auto WebView::hide() -> void {
    if (controller) {
        controller->put_IsVisible(false);
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

auto WebView::test() -> void {
    if (core) {
        // std::wstring title;
        wil::unique_cotaskmem_string buffer;
        core->get_DocumentTitle(&buffer);
        // auto test = std::wstring(buffer.get());
        glow::log::log(L"{}", std::wstring(buffer.get()));
    }
}
}; // namespace glow::webview
