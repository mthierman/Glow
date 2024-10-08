#include <Windows.h>

#include <stdlib.h>

#include <format>
#include <iostream>
#include <print>

#include <glow/glow.hxx>

auto wmain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t* argv[]) -> int {
    std::println("Console example:");

    auto errorCode { S_OK };
    glow::log::log(errorCode);

    auto lambdaFunc = []() {
        auto errorCode { S_OK };
        glow::log::log(errorCode);
    };

    lambdaFunc();

    return EXIT_SUCCESS;
}
