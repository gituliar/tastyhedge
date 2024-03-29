#include "Analytics/Model_BlackScholes.h"

#include "_public/lets_be_rational/lets_be_rational.h"

#include <ql/qldefines.hpp>
#include <ql/exercise.hpp>
#include <ql/instruments/vanillaoption.hpp>
#include <ql/pricingengines/blackscholescalculator.hpp>
#include <ql/pricingengines/vanilla/qdfpamericanengine.hpp>
#include <ql/quotes/simplequote.hpp>
#include <ql/termstructures/volatility/equityfx/blackconstantvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/time/calendars/target.hpp>

#include "tracy/Tracy.hpp"

namespace ql = QuantLib;

using namespace tasty;


Error
Model_BlackScholes::calibrate(const Quote& quote, const RateCurve* rateCurve)
{
    f64 r = rateCurve->rate(Duration(quote.dte, Period_Day));
    f64 q = 0;

    switch (quote.exercise) {
    case Exercise_American:
        if (auto err = calibrateAmerican(quote.callBid, quote.stockPrice, quote.strike, quote.dte, r, q, Parity_Call, m_params.callIvBid); !err.empty())
            return "Model_BlackScholes::calibrate : " + err;
        if (auto err = calibrateAmerican(quote.callAsk, quote.stockPrice, quote.strike, quote.dte, r, q, Parity_Call, m_params.callIvAsk); !err.empty())
            return "Model_BlackScholes::calibrate : " + err;
        if (auto err = calibrateAmerican(quote.putBid, quote.stockPrice, quote.strike, quote.dte, r, q, Parity_Put, m_params.putIvBid); !err.empty())
            return "Model_BlackScholes::calibrate : " + err;
        if (auto err = calibrateAmerican(quote.putAsk, quote.stockPrice, quote.strike, quote.dte, r, q, Parity_Put, m_params.putIvAsk); !err.empty())
            return "Model_BlackScholes::calibrate : " + err;

        return "";
    case Exercise_European:
        if (auto err = calibrateEuropean(quote.putAsk, quote.stockPrice, quote.strike, quote.dte, r, q, Parity_Put, m_params.putIvAsk); !err.empty())
            return "Model_BlackScholes::calibrate : " + err;

        return "";
    }

    return "Model_BlackScholes::calibrate : Unknown exercise";
}


Error
Model_BlackScholes::calibrateAmerican(f64 v_, f64 s, f64 k, f64 dte, f64 r, f64 q, Parity w, f64& z)
{
    ZoneScoped;

    Error err;

    f64 v = v_;
    f64 dv = 0.005;
    f64 dz = 0.001;
    i16 n = 16;

    /// Initial guess
    ///
    if (err = calibrateEuropean(v_, s, k, dte, r, q, w, z); !err.empty())
        return "Model_BlackScholes::calibrateAmerican : " + err;

    /// Newton-Raphson solver
    ///
    while  (n-- > 0 && !std::isnan(z)) {
        if (err = priceAmerican(s, k, dte, z, r, q, w, v); !err.empty())
            return "Model_BlackScholes::calibrateAmerican : " + err;
        if (std::isnan(v))
            break;

        if (std::abs(v - v_) < dv)
            /// Solution found
            return "";

        f64 vUp;
        if (err = priceAmerican(s, k, dte, z + dz, r, q, w, vUp); !err.empty())
            break;

        f64 dvdz = (vUp - v) / dz;
        z -= (v - v_) / dvdz;
    }

    /// No solution
    ///
    z = NaN;
    return "";
}


Error
Model_BlackScholes::calibrateEuropean(f64 v, f64 s, f64 k, f64 dte, f64 r, f64 q, Parity w, f64& z)
{
    ZoneScoped;

    f64 t = f64(dte) / kDaysInYear;
    f64 k_ = k * exp(-t * r);

    f64 intrinsic = std::max<f64>(0, (w == Parity_Call) ? s - k_ : k - s);
    f64 extrinsic = v - intrinsic;
    if (extrinsic >= 0)
    {
        z = implied_volatility_from_a_transformed_rational_guess(v, s, k_, t, w);
    }
    else
    {
        v = intrinsic - extrinsic;
        z = -1 * implied_volatility_from_a_transformed_rational_guess(v, s, k_, t, w);
    }

    return "";
}


Error
Model_BlackScholes::priceAmerican(f64 s, f64 k, f64 dte, f64 z, f64 r, f64 q, Parity w, f64& v) const
{
    /// Anchor + Maturity
    ///
    auto anchor = ql::Date(31, ql::Jul, 1944);
    auto act365 = ql::Actual365Fixed();
    auto maturity = anchor + std::ceil(dte);

    ql::Settings::instance().evaluationDate() = anchor;

    /// Option Data
    ///
    ql::Option::Type
        w_ = (w == Parity_Call) ? ql::Option::Call : ql::Option::Put;

    ql::Handle<ql::YieldTermStructure>
        r_(ql::ext::make_shared<ql::FlatForward>(anchor, r, act365));

    ql::Handle<ql::YieldTermStructure>
        q_(ql::ext::make_shared<ql::FlatForward>(anchor, q, act365));

    ql::Handle<ql::Quote>
        s_(ql::ext::make_shared<ql::SimpleQuote>(s));

    ql::Handle<ql::BlackVolTermStructure>
        z_(ql::ext::make_shared<ql::BlackConstantVol>(anchor, ql::TARGET(), abs(z), act365));

    /// Black-Scholes Model
    ///
    auto bsm = ql::ext::make_shared<ql::BlackScholesMertonProcess>(s_, q_, r_, z_);
    auto engine = ql::ext::make_shared<ql::QdFpAmericanEngine>(
        bsm, ql::QdFpAmericanEngine::fastScheme());

    auto payoff = ql::ext::make_shared<ql::PlainVanillaPayoff>(w_, k);
    auto americanExercise = ql::ext::make_shared<ql::AmericanExercise>(anchor, maturity);
    ql::VanillaOption americanOption(payoff, americanExercise);

    americanOption.setPricingEngine(engine);

    /// Boundary-Interpolation Pricer
    ///
    try {
        ZoneScoped;

        v = americanOption.NPV();
        if (z < 0)
        {
            f64 k_ = k * exp(-dte * r / kDaysInYear);
            v = 2 * std::max<f64>(0, (w == Parity_Call) ? s - k_ : k - s) - v;
        }
    }
    catch (...) {
        std::exception_ptr ep = std::current_exception();
        try {
            std::rethrow_exception(ep);
        }
        catch (std::exception& e) {
            return "Model_BlackScholes::priceAmerican : "s + e.what();
        }
    }

    return "";
}
