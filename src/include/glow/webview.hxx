// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <wrl.h>

#include <wil/com.h>
#include <wil/resource.h>
#include <wil/wrl.h>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <filesystem>
#include <functional>
#include <string>

namespace glow::webview {
struct Environment {
    using Callback = std::function<::HRESULT()>;

    auto create(Callback callback = 0) -> ::HRESULT;
    auto close() -> void;

    struct Options {
        std::string AdditionalBrowserArguments;
        bool AllowSingleSignOnUsingOSPrimaryAccount { false };
        std::string Language;
        std::string TargetCompatibleBrowserVersion;
        bool ExclusiveUserDataFolderAccess { false };
        bool IsCustomCrashReportingEnabled { false };
        bool EnableTrackingPrevention { true };
        bool AreBrowserExtensionsEnabled { false };
        COREWEBVIEW2_CHANNEL_SEARCH_KIND ChannelSearchKind {
            COREWEBVIEW2_CHANNEL_SEARCH_KIND::COREWEBVIEW2_CHANNEL_SEARCH_KIND_MOST_STABLE
        };
        COREWEBVIEW2_SCROLLBAR_STYLE ScrollBarStyle {
            COREWEBVIEW2_SCROLLBAR_STYLE::COREWEBVIEW2_SCROLLBAR_STYLE_DEFAULT
        };
        std::filesystem::path browserExecutableFolder;
        std::filesystem::path userDataFolder;
    };
    Options options;

    wil::com_ptr<ICoreWebView2Environment13> environment;
};

// struct WebView {
//     auto create(const Environment& environment, ::HWND hwnd, std::function<void()> callback =
//     []() {
//     }) -> ::HRESULT;
//     auto close() -> void;
//     auto put_bounds(::RECT rect) -> void;
//     auto put_bounds(::SIZE size) -> void;
//     auto put_bounds(::WINDOWPOS windowPos) -> void;
//     auto put_bounds(::HWND hwnd) -> void;
//     auto show() -> void;
//     auto hide() -> void;
//     auto navigate(const std::string& url) -> void;
//     auto navigate(const std::wstring& url) -> void;
//     auto test() -> void;

//     struct Options {
//         bool AreBrowserAcceleratorKeysEnabled { true };
//         bool AreDefaultContextMenusEnabled { true };
//         bool AreDefaultScriptDialogsEnabled { true };
//         bool AreDevToolsEnabled { true };
//         bool AreHostObjectsAllowed { true };
//         COREWEBVIEW2_PDF_TOOLBAR_ITEMS HiddenPdfToolbarItems {
//             COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE
//         };
//         bool IsBuiltInErrorPageEnabled { true };
//         bool IsGeneralAutofillEnabled { true };
//         bool IsNonClientRegionSupportEnabled { true };
//         bool IsPasswordAutosaveEnabled { true };
//         bool IsPinchZoomEnabled { true };
//         bool IsReputationCheckingRequired { true };
//         bool IsScriptEnabled { true };
//         bool IsStatusBarEnabled { true };
//         bool IsSwipeNavigationEnabled { true };
//         bool IsWebMessageEnabled { true };
//         bool IsZoomControlEnabled { true };
//     };
//     Options options;

//     wil::com_ptr<ICoreWebView2Controller4> controller;
//     wil::com_ptr<ICoreWebView2_22> core;
//     wil::com_ptr<ICoreWebView2Settings9> settings;
// };
}; // namespace glow::webview
