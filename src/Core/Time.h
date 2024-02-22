#pragma once

#include <chrono>
#include <ostream>

#include "Core/Core.h"


namespace tasty
{

enum Period : u8
{
    Period_Second,
    Period_Minute,
    Period_Hour,
    Period_Day,
    Period_Month,
    Period_Year
};


class Time;

class Duration
{
    friend Time;

private:
    u64 m_rep;
    Period
        m_period;

public:
    Duration() :
        m_rep{ 0 },
        m_period{ Period_Minute }
    {};

    Duration(u64 rep, Period period) :
        m_rep{ rep },
        m_period{ period }
    {};

    static Error
        fromString(const string_view str, Duration& dt);
    string
        toString() const;

    u64 seconds() const;
    const Period&
        period() const { return m_period; }
    const u64&
        rep() const { return m_rep; }

    bool operator<(const Duration& dt) const;
    bool operator>(const Duration& dt) const;
    bool operator<=(const Duration& dt) const;
    bool operator>=(const Duration& dt) const;

    bool operator==(const Duration& dt) const;
    bool operator!=(const Duration& dt) const;

    operator bool() const;
};

namespace duration_literals {
    Duration operator""_s(unsigned long long int rep);
    Duration operator""_m(unsigned long long int rep);
    Duration operator""_h(unsigned long long int rep);
    Duration operator""_d(unsigned long long int rep);
    Duration operator""_M(unsigned long long int rep);
    Duration operator""_Y(unsigned long long rep);
}

using namespace tasty::duration_literals;


class Time
{
private:
    u16 m_year;
    u8  m_month;
    u8  m_day;
    u8  m_hour;
    u8  m_minute;
    u8  m_second;

public:
    explicit
    Time(u16 year = 1900, u8 month = 1, u8 day = 0, u8 hour = 0, u8 minute = 0, u8 second = 0) :
        m_year{ year }, m_month{ month }, m_day{ day }, m_hour{ hour }, m_minute{ minute }, m_second{ second }
    {};

    static Error
        fromDateString(const string& s, Time& t);
    static Error
        fromMDYString(const string_view& s, Time& t);
    static Error
        fromTsDateString(const string_view s, Time& t);
    static Error
        fromTsString(const string_view& s, Time& t);

    const auto&
        year() const { return m_year; }
    const auto&
        month() const { return m_month; }
    const auto&
        day() const { return m_day; }
    const auto&
        hour() const { return m_hour; }
    const auto&
        minute() const { return m_minute; }

    auto&
        year() { return m_year; }
    auto&
        month() { return m_month; }
    auto&
        day() { return m_day; }
    auto&
        hour() { return m_hour; }
    auto&
        minute() { return m_minute; }


    string
        toDateString() const;
    string
        toDateTimeString() const;
    string
        toIsoString() const;
    string
        toTsDateString() const;
    string
        toTsString() const;
    string
        toTsTimeString() const;

    Time
        date() const { return Time(m_year, m_month, m_day, 0, 0, 0); }
    std::pair<Time, Time>
        frameTime(const Duration& dt) const;
    bool
        isWeekend() const;
    bool
        inSameFrame(const Time& t, const Duration& dt) const;


    bool operator<(const Time& t) const;
    bool operator>(const Time& t) const;
    bool operator<=(const Time& t) const;
    bool operator>=(const Time& t) const;

    bool operator==(const Time& t) const;
    bool operator!=(const Time& t) const;

    Time
        operator+(const Duration& dt) const;
    Time&
        operator+=(const Duration& dt);
    Time
        operator-(const Duration& dt) const;
    Time&
        operator-=(const Duration& dt);

    bool
        empty() const;

    //operator bool() const;

    friend std::ostream& operator<<(std::ostream& os, const Time& t) {
        return os << "Time(" << t.m_year << "," << int(t.m_month) << "," << int(t.m_day) << ", "
                  << int(t.m_hour) << "," << int(t.m_minute) << "," << int(t.m_second) << ")";
    }


private:
    string
        yearToString() const;
    string
        monthToString() const;
    string
        dayToString() const;
    string
        hourToString() const;
    string
        minuteToString() const;
    string
        secondToString() const;

    void
        fromJulianDay(const u64& seconds);
    u64
        toJulianDay() const;
};


class Timer
{
private:
    std::chrono::time_point<std::chrono::steady_clock>
        m_origin;

public:
    Timer();

    f64
        elapsed_time_sec() const;
    void
        reset();
};

}
