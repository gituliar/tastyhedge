#pragma once

#include "Analytics/Asset.h"
#include "Core/Csv.h"
#include "Core/Time.h"


namespace tasty
{

struct Quote
{
    Time
        ts;
    Ticker
        ticker;
    f32 stockPrice;

    Expiry
        expiry;
    Strike
        strike;
    Exercise
        exercise;

    u16 dte;

    f32 callBid;
    f32 callAsk;

    f32 putBid;
    f32 putAsk;

public:
    Error
        fromCsv(const CsvReader& csv);
};



using Tape = vector<Quote>;

Error
    loadTape(const fs::path& srcPath, Tape& tape);

}
