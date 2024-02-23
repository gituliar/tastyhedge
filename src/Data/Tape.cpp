#include "Data/Tape.h"

#include "Core/Csv.h"
#include "Core/Logger.h"
#include "Core/String.h"

using namespace tasty;



Error
Quote::fromCsv(const CsvReader& src)
{
    Error err;

    if (err = src.get(kCsv_ts_close, ts); !err.empty()) goto error;

    if (err = src.get(kCsv_ticker, ticker); !err.empty()) goto error;

    if (err = src.get(kCsv_stockPrice, stockPrice); !err.empty())
        return "Quote::fromCsv : stockPrice : " + err;

    if (err = src.get(kCsv_expirDate, expiry); !err.empty()) goto error;

    exercise = Exercise_American;

    if (err = src.get(kCsv_strike, strike); !err.empty())
        return "Quote::fromCsv : strike : " + err;

    if (err = src.get(kCsv_dte, dte); !err.empty())
        return "Quote::fromCsv : dte : " + err;

    if (err = src.get(kCsv_callBidPrice, callBid); !err.empty())
        return "Quote::fromCsv : callBidPrice : " + err;
    if (err = src.get(kCsv_callAskPrice, callAsk); !err.empty())
        return "Quote::fromCsv : callAskPrice : " + err;
    if (err = src.get(kCsv_putBidPrice, putBid); !err.empty())
        return "Quote::fromCsv : putBidPrice : " + err;
    if (err = src.get(kCsv_putAskPrice, putAsk); !err.empty())
        return "Quote::fromCsv : putAskPrice : " + err;

    return "";

error:
    return "Quote::fromCsv : " + err;
}




Error
tasty::loadTape(const fs::path& srcPath, Tape& tape)
{
    LOG(INFO) << srcPath;

    Error err;

    CsvReader src;
    if (auto err = src.open(srcPath); !err.empty())
        return "Tape::load : " + err;

    std::string currTicker;
    while (src.readline())
    {
        auto& quote = tape.emplace_back();

        if (auto err = quote.fromCsv(src); !err.empty())
            return "Tape::load : " + err;
    }

    return "";
}
