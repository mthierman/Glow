#include <Windows.h>

#include <stdlib.h>

#include <glow/glow.hxx>

#include <print>

#include <icu.h>

auto wmain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t* argv[]) -> int {
    auto text { std::u16string(u"test") };
    // std::println("text - length: {} size: {} string: {}", text.length(), text.size(), text);

    auto view = std::u16string_view(text);
    // std::println("view - length: {} size: {}", view.length(), view.size());

    std::u16string buffer;
    UErrorCode errorCode { U_ZERO_ERROR };
    buffer.resize(
        u_strToUpper(nullptr, 0, view.data(), static_cast<int32_t>(view.length()), 0, &errorCode));

    errorCode = U_ZERO_ERROR;
    u_strToUpper(buffer.data(), 4, view.data(), static_cast<int32_t>(view.length()), 0, &errorCode);

    std::println("{}", buffer);

    return EXIT_SUCCESS;
}
