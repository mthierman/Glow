// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "url.hxx"
#include "text.hxx"

#include <ada.h>

// https://github.com/chromium/chromium/tree/main/url
// https://stackoverflow.com/questions/736513/how-do-i-parse-a-url-into-hostname-and-path-in-javascript
// https://www.ada-url.com/docs/clients/cpp
// https://stackoverflow.com/questions/74592166/parse-url-without-protocol-in-node-using-constructor

namespace glow
{
auto parse_url(std::string url) -> std::string
{
    url = glow::trim(url);

    if (url.length() == 0) { return url; }

    else if (url.starts_with("http"))
    {
        if (auto parsed{ada::parse(url)}; parsed)
        {
            return {parsed->get_href().begin(), parsed->get_href().end()};
        }
    }

    else if (url.contains("."))
    {
        if (auto parsed{ada::parse("https://" + url)}; parsed)
        {
            return {parsed->get_href().begin(), parsed->get_href().end()};
        }
    }

    else if (auto parsed{ada::parse("https://www.google.com")}; parsed)
    {
        parsed->set_search(url);
        return {parsed->get_href().begin(), parsed->get_href().end()};
    }

    return {};
}
} // namespace glow
