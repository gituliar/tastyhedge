#pragma once

#include <map>

#include "Core/Csv.h"
#include "Core/Time.h"
#include "Data/Navigator.h"


namespace tasty
{

class RateCurve
{
private:
    Time
        m_anchor;
    vector<Duration>
        m_terms;
    vector<f32>
        m_rates;

public:
    RateCurve() {};

    RateCurve(const Time& anchor, const vector<Duration>& terms, const vector<f32>& rates) :
        m_anchor{ anchor }, m_terms{ terms }, m_rates{ rates }
    {};

    Error
        fromCsv(const CsvReader& src);

    const Time&
        anchor() const { return m_anchor; }

    f32 rate(const Duration& term) const;
};



/// Rates taken from <https://home.treasury.gov/policy-issues/financing-the-government/interest-rate-statistics>
///
class RatesHub
{
private:
    std::map<Time, RateCurve>
        m_curves;

public:
    Error
        init(const fs::path& rootDir);

    f64 rate(const Time& anchor, const Duration& term) const;

    const RateCurve*
        rateCurve(const Time& anchor) const;

private:
    Error
        load(const fs::path& srcPath);
};

}