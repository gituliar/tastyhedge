#pragma once

#include <cstdlib>
#include <type_traits>

#include "Core/FileIO.h"
#include "Core/String.h"
#include "Core/Time.h"


namespace tasty
{

enum kCsv_ : u8 {
    kCsv_price_open,
    kCsv_price_close,
    kCsv_price_min,
    kCsv_price_max,
    kCsv_ticker,
    kCsv_expiry,
    kCsv_strike,
    kCsv_dte,
    kCsv_ts_open,
    kCsv_ts_close,
    kCsv_call_bid,
    kCsv_call_ask,
    kCsv_put_bid,
    kCsv_put_ask,
    kCsv_delta,
    kCsv_gamma,
    kCsv_theta,
    kCsv_vega,
    kCsv_call_iv_bid,
    kCsv_call_iv_ask,
    kCsv_call_iv_mid,
    kCsv_put_iv_bid,
    kCsv_put_iv_ask,
    kCsv_put_iv_mid,
    kCsv_call_volume_close,
    kCsv_put_volume_close,
    kCsv_call_volume,
    kCsv_put_volume,
    kCsv_call_open_interest,
    kCsv_put_open_interest,

    kCsv_stockPrice,
    kCsv_expirDate,
    kCsv_callVolume,
    kCsv_callOpenInterest,
    kCsv_callBidPrice,
    kCsv_callAskPrice,
    kCsv_callMidIv,
    kCsv_putVolume,
    kCsv_putOpenInterest,
    kCsv_putBidPrice,
    kCsv_putAskPrice,
    kCsv_putMidIv,

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
    kCsv_30Y,

    kCsv_Size
};

static const vector<string_view>
kCsvNames = {
    "price_open",
    "price_close",
    "price_min",
    "price_max",
    "ticker",
    "expiry",
    "strike",
    "dte",
    "ts_open",
    "ts_close",
    "call_bid",
    "call_ask",
    "put_bid",
    "put_ask",
    "delta",
    "gamma",
    "theta",
    "vega",
    "call_iv_bid",
    "call_iv_ask",
    "call_iv_mid",
    "put_iv_bid",
    "put_iv_ask",
    "put_iv_mid",
    "call_volume_close",
    "put_volume_close",
    "call_volume",
    "put_volume",
    "call_open_interest",
    "put_open_interest",

    "stockPrice",
    "expirDate",
    "callVolume",
    "callOpenInterest",
    "callBidPrice",
    "callAskPrice",
    "callMidIv",
    "putVolume",
    "putOpenInterest",
    "putBidPrice",
    "putAskPrice",
    "putMidIv",

    "Date",
    "\"1 Mo\"",
    "\"2 Mo\"",
    "\"3 Mo\"",
    "\"4 Mo\"",
    "\"6 Mo\"",
    "\"1 Yr\"",
    "\"2 Yr\"",
    "\"3 Yr\"",
    "\"5 Yr\"",
    "\"7 Yr\"",
    "\"10 Yr\"",
    "\"20 Yr\"",
    "\"30 Yr\"",
};


class CsvReader
{

private:
    Ptr<FileReader>
        m_src;
    string
        m_header;
    string_view
        m_line;

    vector<string_view>
        m_buf;
    i16 m_col[kCsv_Size];

public:
    CsvReader() :
        m_buf{ kCsv_Size } {};

    bool
        eof() const { return m_src->eof(); };
    string_view
        header() const { return m_header; };
    string_view
        line() const { return m_line; };

    bool
        contains(u8 id) const { return m_col[id] >= 0; };

    Error
        get(const kCsv_ id, Time& value) const;
    Error
        getMDY(const kCsv_ id, Time& value) const;

    template<typename T>
    Error
        get(const kCsv_ id, T& value) const
    {
        
        string_view sv;
        if (auto i = m_col[id]; i < 0)
            return "CsvReader::get : Missing '" + string(kCsvNames[id]) + "' column";
        else
            sv = m_buf[i];

        if constexpr (std::is_arithmetic_v<T>) {
            if (auto err = fromString(sv, value); !err.empty())
                return "CsvReader::get : " + err;
        }
        else {
            try {
                value = T(sv);
            }
            catch (const std::exception& ex) {
                LOG(WARN) << "Fail to parse: " << m_line;
                return "CsvReader::get : " + string(ex.what());
            }
        }

        return "";
    }

    Error
        close();

    Error
        open(const fs::path& path);
    Error
        readline();
};



class CsvWriter
{
private:
    Ptr<FileWriter>
        m_dst;

    string
        m_header;
    string
        m_line;
    u32 m_n = 0;

public:
    Error
        close();
    Error
        open(const fs::path& path);

    void
        newline()
    {
        if (m_n++ == 0)
            writeline(m_header);

        writeline(m_line);
        m_line.clear();
    };

    void
        put(const kCsv_ id, const f32& value, const u8 n)
    {
        put(id, toString(value, n));
    };
    void
        put(const kCsv_ id, const string_view& value)
    {
        if (m_n == 0) {
            if (!m_header.empty())
                m_header += ',';
            m_header += kCsvNames[id];
        };

        if (!m_line.empty())
            m_line += ',';
        m_line += value;
    };
    void
        put(const kCsv_ id, const Time& value)
    {
        put(id, value.toTsString());
    };

    Error
        writeline(const string_view& line);
};

}
