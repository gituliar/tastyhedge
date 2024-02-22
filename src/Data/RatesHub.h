#pragma once

#include <map>

#include "Core/Csv.h"
#include "Core/Time.h"
#include "Data/Navigator.h"


namespace tasty
{

class RateCurve
{
public:
    static const auto&
        kCsv_s()
    {
        static const vector<kCsv_> columns =
        {
            kCsv_Date,
            kCsv_1M,
            kCsv_2M,
            kCsv_3M,
            kCsv_4M,
            kCsv_6M,
            kCsv_1Y,
            kCsv_2Y,
            kCsv_3Y,
            kCsv_5Y,
            kCsv_7Y,
            kCsv_10Y,
            kCsv_20Y,
            kCsv_30Y
        };
        return columns;
    };

private:
    Time
        m_anchor;
    vector<Duration>
        m_terms;
    vector<f32>
        m_rates;

    f64 m_rate;

public:
    RateCurve() : m_rate{ NaN } {};

    RateCurve(const f64 rate) : m_rate{ rate } {};

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