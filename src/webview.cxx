// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/webview.hxx>

namespace glow::webview {
// auto Environment::create(Callback callback) -> ::HRESULT {
//     wil::com_ptr<ICoreWebView2EnvironmentOptions> createdEnvironmentOptions {
//         Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>()
//     };
//     wil::com_ptr<ICoreWebView2EnvironmentOptions2> createdEnvironmentOptions2;
//     wil::com_ptr<ICoreWebView2EnvironmentOptions3> createdEnvironmentOptions3;
//     wil::com_ptr<ICoreWebView2EnvironmentOptions4> createdEnvironmentOptions4;
//     wil::com_ptr<ICoreWebView2EnvironmentOptions5> createdEnvironmentOptions5;
//     wil::com_ptr<ICoreWebView2EnvironmentOptions6> createdEnvironmentOptions6;
//     wil::com_ptr<ICoreWebView2EnvironmentOptions7> createdEnvironmentOptions7;
//     wil::com_ptr<ICoreWebView2EnvironmentOptions8> createdEnvironmentOptions8;

//     createdEnvironmentOptions2
//         = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions2>();
//     createdEnvironmentOptions3
//         = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions3>();
//     createdEnvironmentOptions4
//         = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions4>();
//     createdEnvironmentOptions5
//         = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions5>();
//     createdEnvironmentOptions6
//         = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions6>();
//     createdEnvironmentOptions7
//         = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions7>();
//     createdEnvironmentOptions8
//         = createdEnvironmentOptions.try_query<ICoreWebView2EnvironmentOptions8>();

//     if (!environmentOptions.AdditionalBrowserArguments.empty()) {
//         createdEnvironmentOptions->put_AdditionalBrowserArguments(
//             glow::text::to_wstring(environmentOptions.AdditionalBrowserArguments).c_str());
//     }

//     createdEnvironmentOptions->put_AllowSingleSignOnUsingOSPrimaryAccount(
//         environmentOptions.AllowSingleSignOnUsingOSPrimaryAccount);

//     if (!environmentOptions.Language.empty()) {
//         createdEnvironmentOptions->put_Language(
//             glow::text::to_wstring(environmentOptions.Language).c_str());
//     }

//     if (!environmentOptions.TargetCompatibleBrowserVersion.empty()) {
//         createdEnvironmentOptions->put_TargetCompatibleBrowserVersion(
//             glow::text::to_wstring(environmentOptions.TargetCompatibleBrowserVersion).c_str());
//     }

//     createdEnvironmentOptions2->put_ExclusiveUserDataFolderAccess(
//         environmentOptions.ExclusiveUserDataFolderAccess);
//     createdEnvironmentOptions3->put_IsCustomCrashReportingEnabled(
//         environmentOptions.IsCustomCrashReportingEnabled);
//     // createdEnvironmentOptions4->SetCustomSchemeRegistrations();
//     createdEnvironmentOptions5->put_EnableTrackingPrevention(
//         environmentOptions.EnableTrackingPrevention);
//     createdEnvironmentOptions6->put_AreBrowserExtensionsEnabled(
//         environmentOptions.AreBrowserExtensionsEnabled);
//     createdEnvironmentOptions7->put_ChannelSearchKind(environmentOptions.ChannelSearchKind);
//     createdEnvironmentOptions8->put_ScrollBarStyle(environmentOptions.ScrollBarStyle);

//     return CreateCoreWebView2EnvironmentWithOptions(
//         environmentOptions.browserExecutableFolder.c_str(),
//         environmentOptions.userDataFolder.c_str(),
//         createdEnvironmentOptions.get(),
//         wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
//             [this, callback { std::move(callback) }](
//                 ::HRESULT /* errorCode */,
//                 ICoreWebView2Environment* createdEnvironment) -> ::HRESULT {
//         environment = wil::com_ptr<ICoreWebView2Environment>(createdEnvironment)
//                           .try_query<ICoreWebView2Environment13>();

//         if (callback) {
//             callback();
//         }

//         return S_OK;
//     }).Get());
// }

// auto Environment::close() -> void { environment.reset(); }

// auto WebView::create(const Environment& environment, ::HWND hwnd, Callback callback) -> ::HRESULT
// {
//     return environment.environment->CreateCoreWebView2Controller(
//         hwnd,
//         wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
//             [this, callback { std::move(callback) }, hwnd](
//                 ::HRESULT /* errorCode */,
//                 ICoreWebView2Controller* createdController) -> ::HRESULT {
//         controller = wil::com_ptr<ICoreWebView2Controller>(createdController)
//                          .try_query<ICoreWebView2Controller4>();
//         controller->put_DefaultBackgroundColor(webViewSettings.backgroundColor);

//         wil::com_ptr<ICoreWebView2> createdCore;
//         controller->get_CoreWebView2(createdCore.put());
//         core = createdCore.try_query<ICoreWebView2_22>();

//         wil::com_ptr<ICoreWebView2Settings> createdSettings;
//         core->get_Settings(createdSettings.put());
//         settings = createdSettings.try_query<ICoreWebView2Settings9>();

//         settings->put_AreBrowserAcceleratorKeysEnabled(
//             webViewSettings.AreBrowserAcceleratorKeysEnabled);
//         settings->put_AreDefaultContextMenusEnabled(webViewSettings.AreDefaultContextMenusEnabled);
//         settings->put_AreDefaultScriptDialogsEnabled(
//             webViewSettings.AreDefaultScriptDialogsEnabled);
//         settings->put_AreDevToolsEnabled(webViewSettings.AreDevToolsEnabled);
//         settings->put_AreHostObjectsAllowed(webViewSettings.AreHostObjectsAllowed);
//         settings->put_HiddenPdfToolbarItems(webViewSettings.HiddenPdfToolbarItems);
//         settings->put_IsBuiltInErrorPageEnabled(webViewSettings.IsBuiltInErrorPageEnabled);
//         settings->put_IsGeneralAutofillEnabled(webViewSettings.IsGeneralAutofillEnabled);
//         settings->put_IsNonClientRegionSupportEnabled(
//             webViewSettings.IsNonClientRegionSupportEnabled);
//         settings->put_IsPasswordAutosaveEnabled(webViewSettings.IsPasswordAutosaveEnabled);
//         settings->put_IsPinchZoomEnabled(webViewSettings.IsPinchZoomEnabled);
//         settings->put_IsReputationCheckingRequired(webViewSettings.IsReputationCheckingRequired);
//         settings->put_IsScriptEnabled(webViewSettings.IsScriptEnabled);
//         settings->put_IsStatusBarEnabled(webViewSettings.IsStatusBarEnabled);
//         settings->put_IsSwipeNavigationEnabled(webViewSettings.IsSwipeNavigationEnabled);
//         settings->put_IsWebMessageEnabled(webViewSettings.IsWebMessageEnabled);
//         settings->put_IsZoomControlEnabled(webViewSettings.IsZoomControlEnabled);

//         ::RECT rect;
//         ::GetClientRect(hwnd, &rect);
//         put_bounds(rect);

//         if (callback) {
//             callback();
//         }

//         return S_OK;
//     }).Get());
// }

// auto WebView::close() -> void {
//     settings.reset();
//     core.reset();
//     controller.reset();
// }

// auto WebView::put_bounds(const glow::window::Position& position) -> void {
//     if (controller) {
//         controller->put_Bounds(glow::window::to_rect(position));
//     }
// }

// auto WebView::put_bounds(const ::RECT& rect) -> void {
//     if (controller) {
//         controller->put_Bounds(rect);
//     }
// }

// auto WebView::put_bounds(const ::SIZE& size) -> void {
//     if (controller) {
//         controller->put_Bounds(glow::window::to_rect(size));
//     }
// }

// auto WebView::put_bounds(const ::WINDOWPOS& windowPos) -> void {
//     if (controller) {
//         controller->put_Bounds(glow::window::to_rect(windowPos));
//     }
// }

// template <typename T> auto WebView::put_bounds(const T& window) -> void {
//     if (controller) {
//         controller->put_Bounds(window.client_rect());
//     }
// }

// auto WebView::show() -> void {
//     if (controller) {
//         controller->put_IsVisible(true);
//     }
// }

// auto WebView::hide() -> void {
//     if (controller) {
//         controller->put_IsVisible(false);
//     }
// }

// auto WebView::navigate(const std::string& url) -> void {
//     if (core) {
//         core->Navigate(glow::text::to_wstring(url).c_str());
//     }
// }

// auto WebView::navigate(const std::wstring& url) -> void {
//     if (core) {
//         core->Navigate(url.c_str());
//     }
// }

// auto WebView::test() -> void {
//     if (core) {
//         // std::wstring title;
//         wil::unique_cotaskmem_string buffer;
//         core->get_DocumentTitle(&buffer);
//         // auto test = std::wstring(buffer.get());
//         glow::log::log(L"{}", std::wstring(buffer.get()));
//     }
// }
}; // namespace glow::webview
