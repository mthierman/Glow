// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/webview.hxx>

namespace glow::webview {
// auto Environment::close() -> void { environment.reset(); }

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
