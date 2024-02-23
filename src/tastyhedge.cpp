#include <docopt.h>

#include "Analytics/Model_BlackScholes.h"
#include "Core/Logger.h"
#include "Data/Tape.h"
#include "Data/RatesHub.h"

#include "version.h"


using namespace tasty;


const string g_usage = R"(
tastyhedge - Trading Analytics

Usage:
    tastyhedge calibrate [--mds=<dir>] <src> <dst>

Global Options:
    --mds <dir>         Market Data Store [default: c:/mds]

    --version           Print version
)";



Error
cmdCalibrate(
    const fs::path& srcPath,
    const fs::path& dstPath,
    const fs::path& mdsDir)
{
    RatesHub ratesHub;
    if (auto err = ratesHub.init(mdsDir); !err.empty())
        return "cmdCalibrate: " + err;

    Tape tape;
    if (auto err = loadTape(srcPath, tape); !err.empty())
        return "cmdCalibrate: " + err;

    const auto bufPath = fs::path(dstPath.string() + ".buf");
    if (fs::exists(dstPath)) {
        LOG(INFO) << "SKIP " << dstPath;
        return "";
    }

    CsvWriter dst;
    if (auto err = dst.open(bufPath); !err.empty())
        return "cmdCalibrate: " + err;
    LOG(INFO) << "WRITE " << bufPath;


    Model_BlackScholes model;

    for (const auto& quote : tape) {
        auto rateCurve = ratesHub.rateCurve(quote.ts.date());

        model.calibrate(quote, rateCurve);

        const auto& params = model.params();

        dst.put(kCsv_ts_close, quote.ts);
        dst.put(kCsv_ticker, quote.ticker);
        dst.put(kCsv_stockPrice, quote.stockPrice, 2);

        dst.put(kCsv_strike, quote.strike, 2);
        dst.put(kCsv_expiry, quote.expiry);
        dst.put(kCsv_dte, quote.dte, 0);
        dst.put(kCsv_call_bid, quote.callBid, 2);
        dst.put(kCsv_call_ask, quote.callAsk, 2);
        dst.put(kCsv_put_bid, quote.putBid, 2);
        dst.put(kCsv_put_ask, quote.putAsk, 2);

        dst.put(kCsv_call_iv_bid, params.callIvBid, 4);
        dst.put(kCsv_call_iv_ask, params.callIvAsk, 4);
        dst.put(kCsv_put_iv_bid, params.putIvBid, 4);
        dst.put(kCsv_put_iv_ask, params.putIvAsk, 4);

        dst.newline();
    }

    //auto calibrateJob = [&](const Time anchor, const Ticker& ticker) {
    //    if (auto err = riskEngine.make(anchor, ticker); !err.empty())
    //        LOG(ERROR) << err;
    //};

    //auto& pool = ThreadPool::instance();
    //for (const auto& anchor : anchors) {
    //    for (const auto& ticker : tickers) {
    //        pool.push_task(calibrateJob, anchor, ticker);
    //        //calibrateJob(anchor, ticker);
    //    }
    //}
    //pool.wait_for_tasks();


    if (auto err = dst.close(); !err.empty())
        return "cmdCalibrate : " + err;

    fs::rename(bufPath, dstPath);
    LOG(INFO) << "RENAME " << dstPath;

    return "";
}


Error
tastyhedge(int argc, char **argv)
{
    const auto version = "tastyhedge " + Version::GIT_REV + " (" + Version::BUILD_DATE + ")";

    /// Parse Command-Line Arguments
    ///
    auto args = docopt::docopt(g_usage, { argv + 1, argv + argc }, true, version);

    /// Init Logger
    ///
    Logger::level() = INFO;

    LOG(INFO) << version;

    /// Run CLI Command
    ///
    if (args.at("calibrate").asBool()) {
        const auto srcPath = fs::absolute(args.at("<src>").asString());
        const auto dstPath = fs::absolute(args.at("<dst>").asString());
        const auto mdsDir = fs::absolute(args.at("--mds").asString());

        if (auto err = cmdCalibrate(srcPath, dstPath, mdsDir); !err.empty())
            return "Kwinto::main : " + err;
    }
    else
        return "tastyhedge: unknown command";

    return "";
};



int main(int argc, char** argv)
{
    try {
        if (auto err = tastyhedge(argc, argv); !err.empty()) {
            LOG(ERROR) << err;

            return EXIT_FAILURE;
        };
    }
    catch (const std::exception& ex) {
        LOG(ERROR) << ex.what();
    }

    return EXIT_SUCCESS;
}
