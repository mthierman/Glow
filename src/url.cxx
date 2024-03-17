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

namespace glow
{
auto parse_url(std::string url) -> std::string
{
    url = glow::trim(url);

    if (url.length() == 0) { return url; }

    if (url.starts_with("http"))
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

    else
    {
        if (auto parsed{ada::parse("https://www.google.com")}; parsed)
        {
            parsed->set_search(url);
            return {parsed->get_href().begin(), parsed->get_href().end()};
        }

        return "https://www.google.com/search?q=" + url;
    }
}
} // namespace glow
