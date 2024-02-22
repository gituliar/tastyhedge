#pragma once

#include <ostream>

#include "Core/Core.h"


namespace tasty
{

enum Exercise : u8
{
    Exercise_American,
    Exercise_European
};

enum Parity : i8
{
    Parity_Call = +1,
    Parity_Put  = -1
};


using Expiry = string;
using Strike = f32;
using Ticker = string;


struct Option
{
    f64  t;  //  time to maturity    1.0 = 365 days
    f64  k;  //  strike              100 = $100
    f64  z;  //  volatility          0.2 = 20% per year
    f64  r;  //  risk-free rate      0.05 = 5% per year
    f64  q;  //  dividend yield      0.03 = 3% per year
    f64  s;  //  spot price          100 = $100
    bool e;  //  early exercise      true = American, false = European
    Parity w;  //  put or call

    friend bool operator<(const Option& l, const Option& r)
    {
        return std::tie(l.t, l.k, l.z, l.q, l.r, l.s, l.e, l.w)
            < std::tie(r.t, r.k, r.z, r.q, r.r, r.s, r.e, r.w);
    };
};

std::ostream&
    operator<<(std::ostream& os, const Option& o);

}