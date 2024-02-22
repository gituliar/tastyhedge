#pragma once

#include "Data/RatesHub.h"
#include "Data/Tape.h"


namespace tasty
{

class Model_BlackScholes
{
public:
    struct ModelParams
    {
        f64 callIvBid;
        f64 callIvAsk;
        f64 putIvBid;
        f64 putIvAsk;
    };

private:
    ModelParams
        m_params;

public:
    const ModelParams&
        params() const { return m_params; };

    Error
        calibrate(const Quote& quote, const RateCurve* rateCurvee);

    Error
        calibrateAmerican(f64 v, f64 s, f64 k, f64 dte, f64 r, f64 q, Parity w, f64& z);
    Error
        calibrateEuropean(f64 v, f64 s, f64 k, f64 dte, f64 r, f64 q, Parity w, f64& z);

    Error
        priceAmerican(f64 s, f64 k, f64 dte, f64 z, f64 r, f64 q, Parity w, f64& p) const;
};

}