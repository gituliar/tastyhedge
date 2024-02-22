#include "Data/Tape.h"

#include "Core/Csv.h"
#include "Core/Logger.h"
#include "Core/String.h"

using namespace tasty;



Error
Quote::fromCsv(const CsvReader& src)
{
    Error err;
//
//    m_ts = ts;
//
//    if (err = src.get(kCsv_ticker, m_ticker); !err.empty()) goto error;
//
//    if (err = src.get(kCsv_expirDate, m_expiry); !err.empty()) goto error;
//    m_expiry.erase(7, 1);
//    m_expiry.erase(4, 1);
//
//    if (err = src.get(kCsv_dte, m_dte); !err.empty())
//        return "Quote::fromCsv : dte : " + err;
//
//    if (err = src.get(kCsv_strike, m_strike); !err.empty())
//        return "Quote::fromCsv : strike : " + err;
//
//    if (err = src.get(kCsv_stockPrice, m_stock_price); !err.empty())
//        return "Quote::fromCsv : stockPrice : " + err;
//
//    if (err = src.get(kCsv_callVolume, m_call_volume); !err.empty())
//        return "Quote::fromCsv : callVolume : " + err;
//    if (err = src.get(kCsv_callOpenInterest, m_call_open_interest); !err.empty())
//        return "Quote::fromCsv : callOpenInterst : " + err;
//    if (err = src.get(kCsv_putVolume, m_put_volume); !err.empty())
//        return "Quote::fromCsv : putVolume : " + err;
//    if (err = src.get(kCsv_putOpenInterest, m_put_open_interest); !err.empty())
//        return "Quote::fromCsv : putOpenInterest : " + err;
//
//    if (err = src.get(kCsv_callBidPrice, m_call_bid); !err.empty())
//        return "Quote::fromCsv : callBidPrice : " + err;
//    if (err = src.get(kCsv_callAskPrice, m_call_ask); !err.empty())
//        return "Quote::fromCsv : callAskPrice : " + err;
//
//    if (err = src.get(kCsv_putBidPrice, m_put_bid); !err.empty())
//        return "Quote::fromCsv : putBidPrice : " + err;
//    if (err = src.get(kCsv_putAskPrice, m_put_ask); !err.empty())
//        return "Quote::fromCsv : putAskPrice : " + err;
//
//    if (err = src.get(kCsv_callMidIv, m_call_iv); !err.empty())
//        return "Quote::fromCsv : callMidIv : " + err;
//    if (err = src.get(kCsv_putMidIv, m_put_iv); !err.empty())
//        return "Quote::fromCsv : putMidIv : " + err;
//
//    if (err = src.get(kCsv_delta, m_delta); !err.empty())
//        return "Quote::fromCsv : delta : " + err;
//    if (err = src.get(kCsv_gamma, m_gamma); !err.empty())
//        return "Quote::fromCsv : gamma : " + err;
//    if (err = src.get(kCsv_theta, m_theta); !err.empty())
//        return "Quote::fromCsv : theta : " + err;
//    if (err = src.get(kCsv_vega, m_vega); !err.empty())
//        return "Quote::fromCsv : vega : " + err;
//
//    return "";
//
//error:
    return "Quote::fromCsv : " + err;
}




Error
tasty::loadTape(const fs::path& srcPath, Tape& tape)
{
    LOG(INFO) << srcPath;

    Error err;

    //if (!Navigator::timestamp(srcPath, m_ts, err))
    //    return "Tape::load : " + err;

    CsvReader src;
    if (auto err = src.open(srcPath); !err.empty())
        return "Tape::load : " + err;

    std::string currTicker;
    while (!src.eof())
    {
        auto& quote = tape.emplace_back();

        if (auto err = src.readline(); !err.empty())
            return "Tape::load : " + err;

        if (auto err = quote.fromCsv(src); !err.empty())
            return "Tape::load : " + err;
    }

    return "";
}
