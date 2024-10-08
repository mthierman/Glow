#include <Windows.h>

#include <stdlib.h>

#include <glow/glow.hxx>

#include <print>

#include <icu.h>

auto wmain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t* argv[]) -> int {
    auto text { std::u16string(u"and so i went..") };
    std::println("{}", text);

    std::u16string buffer;
    UErrorCode errorCode { U_ZERO_ERROR };

    auto length
        = u_strToUpper(nullptr, 0, text.c_str(), static_cast<int32_t>(text.size()), 0, &errorCode);

    buffer.resize(length + 1);

    errorCode = U_ZERO_ERROR;

    u_strToUpper(
        buffer.data(), length + 1, text.c_str(), static_cast<int32_t>(text.size()), 0, &errorCode);

    std::println("{}", buffer);

    return EXIT_SUCCESS;
}
