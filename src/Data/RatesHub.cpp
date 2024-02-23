#include "Data/RatesHub.h"

using namespace tasty;


Error
RateCurve::fromCsv(const CsvReader& src)
{
    static const Duration
        terms[] = { 0_s, 1_M, 2_M, 3_M, 4_M, 6_M, 1_Y, 2_Y, 3_Y, 5_Y, 7_Y, 10_Y, 20_Y, 30_Y };

    static const vector<kCsv_>
        columns = { Csv_Date, Csv_1M, Csv_2M, Csv_3M, Csv_4M, Csv_6M, Csv_1Y, Csv_2Y, Csv_3Y,
            Csv_5Y, Csv_7Y, Csv_10Y, Csv_20Y, Csv_30Y };

    if (auto err = src.getMDY(Csv_Date, m_anchor); !err.empty())
        return "RateCurve::fromCsv : Date : " + err;

    m_rates.clear();
    m_terms.clear();
    for (auto i = 1; i < columns.size(); i++) {
        /// It's OK of some columns are missing
        if (!src.contains(columns[i]))
            continue;

        f32 rate;
        if (auto err = src.get(columns[i], rate); !err.empty()) {
            string sv;
            src.get(columns[i], sv);
            if (sv.empty())
                continue; // missing, it's OK

            return "RateCurve::fromCsv : " + terms[i].toString() + " : " + err;
        }

        m_rates.push_back(rate);
        m_terms.push_back(terms[i]);
    }

    return "";
}


f32
RateCurve::rate(const Duration& term) const
{
    const auto ii = std::lower_bound(m_terms.begin(), m_terms.end(), term);

    if (ii == m_terms.end())
        return NaN;

    const auto i = std::distance(m_terms.begin(), ii);
    return m_rates[i] / 100.;
}



Error
RatesHub::load(const fs::path& srcPath)
{
    CsvReader src;
    if (auto err = src.open(srcPath); !err.empty())
        return "RatesHub::load : " + err;

    while (src.readline()) {

        RateCurve curve;
        if (auto err = curve.fromCsv(src); !err.empty())
            return "RatesHub::load : " + err;

        m_curves[curve.anchor()] = curve;
    }
    
    return "";
};



Error
RatesHub::init(const fs::path& mdsDir)
{
    if (!fs::exists(mdsDir))
        return "RatesHub::init : no dir " + mdsDir.string();

    vector<fs::path> srcPaths;
    if (auto err = Navigator(mdsDir).ratesPathAll(srcPaths); !err.empty())
        return "RatesHub::init : " + err;

    for (const auto& srcPath : srcPaths) {
        if (auto err = load(srcPath); !err.empty())
            return "RatesHub::init : " + srcPath.string() + " : " + err;
    }

    return "";
};


f64
RatesHub::rate(const Time& anchor, const Duration& term) const
{
    if (const auto curve = rateCurve(anchor); curve != nullptr)
        return curve->rate(term);

    return NaN;
}

const RateCurve*
RatesHub::rateCurve(const Time& anchor) const
{
    const auto ii = m_curves.find(anchor);
    if (ii == m_curves.end())
        return nullptr;

    return &(ii->second);
}
