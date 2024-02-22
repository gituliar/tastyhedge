#include "Core/Logger.h"

#include <chrono>
#include <iomanip>
#include <thread>
using namespace std::chrono;

using namespace tasty;


constexpr auto kBlack = "\x1b[0;30m";
constexpr auto kRed =  "\x1b[0;31m";
constexpr auto kGreen =  "\x1b[0;32m";
constexpr auto kYellow = "\x1b[0;33m";
constexpr auto kBlue = "\x1b[0;34m";
constexpr auto kPurple = "\x1b[0;35m";
constexpr auto kCyan = "\x1b[0;36m";
constexpr auto kWhite = "\x1b[0;37m";

//constexpr auto kBold = "\x1b[0;1m";
//constexpr auto kUnderlying = "\x1b[0;4m";

constexpr auto kReset = "\x1b[0m";


Logger::Logger(LoggerLevel level_, const char* file, const u64 line, const char* function) :
    m_cout{ std::cout }
{
    if (level_ < level())
        return;

    switch (level_) {
    case INFO:
        m_cout << kGreen;
        break;
    case WARN:
        m_cout << kYellow;
        break;
    case ERROR:
        m_cout << kRed;
        break;
    }

    auto now = system_clock::now();
    auto t = system_clock::to_time_t(now);
    auto tm = std::localtime(&t);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    m_cout << std::put_time(tm, "%H:%M:%S.") << std::setfill('0') << std::setw(3) << ms.count();
    m_cout << " ";
    //m_cout << " - ";

    //m_cout << fs::path(file).filename().string() << ':' << line;
    //m_cout << " - ";

    m_cout << "[" << std::setfill(' ') << std::setw(6) << std::this_thread::get_id() << "] ";

    m_cout << function;
    m_cout << " : ";

};


Logger::~Logger()
{
    m_cout << kReset << std::endl;
}


LoggerLevel&
Logger::level()
{
    static LoggerLevel level;
    return level;
};

