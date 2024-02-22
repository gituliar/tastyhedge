#pragma once

#include <algorithm>
#include <array>
#include <charconv>

#include "Core/Logger.h"


namespace tasty
{

template<typename T>
Error
    fromString(const string_view& src, T& value)
{
    auto first = src.data();
    auto last = src.data() + src.size();

    if (auto [ptr, ec] = std::from_chars(first, last, value); ptr != last || ec != std::errc{})
        return "kw::fromString : Fail to parse '" + string(src) + "'";

    return "";
};


template<typename T>
Error
    toString(string& dst, T value, int n)
{
    std::array<char, 32> buf;

    auto first = buf.data();
    auto last = buf.data() + buf.size();
    if (auto [ptr, ec] = std::to_chars(first, last, value, std::chars_format::fixed, n); ec == std::errc()) {
        dst += string(buf.data(), ptr - buf.data());
    }
    else
        return "kw::toString: " + std::make_error_code(ec).message();
    return "";
};

template<typename T>
string
    toString(T val, int n)
{
    string dst;
    if (auto err = toString(dst, val, n); !err.empty())
    {
        LOG(WARN) << err;
        return "nan";
    }
    return dst;
};

}
