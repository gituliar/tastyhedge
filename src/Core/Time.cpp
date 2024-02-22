#include "Time.h"

#include "Core/String.h"

using namespace tasty;
using namespace tasty::duration_literals;


string
Time::yearToString() const
{
    return std::to_string(m_year);
}

string
Time::monthToString() const
{
    return (m_month < 10 ? "0" : "") + std::to_string(m_month);
}

string
Time::dayToString() const
{
    return (m_day < 10 ? "0" : "") + std::to_string(m_day);
}

string
Time::hourToString() const
{
    return (m_hour < 10 ? "0" : "") + std::to_string(m_hour);
}

string
Time::minuteToString() const
{
    return (m_minute < 10 ? "0" : "") + std::to_string(m_minute);
}

string
Time::secondToString() const
{
    return (m_second < 10 ? "0" : "") + std::to_string(m_second);
}


void
Time::fromJulianDay(const u64& elapsedSeconds)
{
    m_second = elapsedSeconds % 60;
    const auto elapsedMinutes = elapsedSeconds / 60;

    m_minute = elapsedMinutes % 60;
    const auto elapsedHours =  elapsedMinutes / 60;

    m_hour = elapsedHours % 24;
    const auto elapsedDays = elapsedHours / 24;

    // For details see https://web.archive.org/web/20140523201910/http://mathforum.org/library/drmath/view/51907.html
    const int p = elapsedDays + 2483588;
    const int q = 4 * p / 146097;
    const int r = p - (146097 * q + 3) / 4;
    const int s = 4000 * (r + 1) / 1461001;
    const int t = r - 1461 * s / 4 + 31;
    const int u = 80 * t / 2447;
    const int v = u / 11;

    m_day = t - 2447 * u / 80;
    m_month = u + 2 - 12 * v;
    m_year = 100 * (q - 49) + s + v;
}

u64
Time::toJulianDay() const
{
    // For details see https://web.archive.org/web/20140523201910/http://mathforum.org/library/drmath/view/51907.html

    const int m = (m_month - 14) / 12;
    const int y = m_year + 4800;

    u64 elapsedDays = 1461 * (y + m) / 4 + 367 * (m_month - 2 - 12 * m) / 12 - (3 * ((y + m + 100) / 100)) / 4 + m_day - 2447094;

    return  24 * 60 * 60 * elapsedDays + 60 * 60 * m_hour + 60 * m_minute + m_second;
}



u64
Duration::seconds() const
{
    switch (m_period)
    {
    case Period_Second:
        return m_rep;
    case Period_Minute:
        return m_rep * 60;
    case Period_Hour:
        return m_rep * 60 * 60;
    case Period_Day:
        return m_rep * 60 * 60 * 24;
    case Period_Month:
        return m_rep * 60 * 60 * 24 * 30;
    case Period_Year:
        return m_rep * 60 * 60 * 24 * 30 * 12;
    }

    assert(false && "Unkown period");
    return 0;
}


Error
Duration::fromString(const string_view s, Duration& dt)
{
    Error error;

    u64 rep;
    if (error = tasty::fromString(s.substr(0, s.size() - 1), rep); !error.empty())
        return "Duration::fromString : " + error;

    switch (s.back())
    {
    case 's':
        dt = Duration(rep, Period_Second);
        break;
    case 'm':
        dt = Duration(rep, Period_Minute);
        break;
    case 'h':
        dt = Duration(rep, Period_Hour);
        break;
    case 'd':
        dt = Duration(rep, Period_Day);
        break;
    default:
        return "Duration::fromString : Unknown duration placeholder in " + string(s);
    }

    return "";
};


string
Duration::toString() const
{
    string rep = std::to_string(m_rep);

    switch (m_period)
    {
    case Period_Second:
        return rep + "s";
    case Period_Minute:
        return rep + "m";
    case Period_Hour:
        return rep + "h";
    case Period_Day:
        return rep + "d";
    }

    return "nan";
};


bool
Duration::operator<(const Duration& dt) const
{
    return seconds() < dt.seconds();
};
bool
Duration::operator>(const Duration& dt) const
{
    return seconds() > dt.seconds();
};

bool
Duration::operator<=(const Duration& dt) const
{
    return !(*this > dt);
};

bool
Duration::operator>=(const Duration& dt) const
{
    return !(*this < dt);
};

bool
Duration::operator==(const Duration& dt) const
{
    return seconds() == dt.seconds();
};

bool
Duration::operator!=(const Duration& dt) const
{
    return !(*this == dt);
};

Duration::operator bool() const
{
    return m_rep != 0;
};


Duration
duration_literals::operator""_s(unsigned long long int rep)
{
    return Duration(rep, Period_Second);
}

Duration
duration_literals::operator""_m(unsigned long long int rep)
{
    return Duration(rep, Period_Minute);
}

Duration
duration_literals::operator""_h(unsigned long long int rep)
{
    return Duration(rep, Period_Hour);
}

Duration
duration_literals::operator""_d(unsigned long long int rep)
{
    return Duration(rep, Period_Day);
}

Duration
duration_literals::operator""_M(unsigned long long int rep)
{
    return Duration(rep, Period_Month);
}

Duration
duration_literals::operator""_Y(unsigned long long rep)
{
    return Duration(rep, Period_Year);
}



Error
Time::fromTsDateString(const string_view s, Time& t)
{
    Error error;

    if (s.length() != 8)
        return "Time::fromTsDateString : invalid input " + string(s);

    if (error = fromString(s.substr(0, 4), t.m_year); !error.empty())
        return "Time::fromTsDateString : " + error;

    if (error = fromString(s.substr(4, 2), t.m_month); !error.empty())
        return "Time::fromTsDateString : " + error;

    if (error = fromString(s.substr(6, 2), t.m_day); !error.empty())
        return "Time::fromTsDateString : " + error;

    t.m_hour = 0;
    t.m_minute = 0;
    t.m_second = 0;

    return "";
};

Error
Time::fromDateString(const string& s, Time& t)
{
    Error error;

    if (s.length() != 10)
        return "Time::fromDateString : invalid input " + s;

    if (error = fromString(s.substr(0, 4), t.m_year); !error.empty())
        return "Time::fromDateString : " + error;

    if (error = fromString(s.substr(5, 2), t.m_month); !error.empty())
        return "Time::fromDateString : " + error;

    if (error = fromString(s.substr(8, 2), t.m_day); !error.empty())
        return "Time::fromDateString : " + error;

    t.m_hour = 0;
    t.m_minute = 0;
    t.m_second = 0;

    return "";
}

Error
Time::fromMDYString(const string_view& sv, Time& t)
{
    if (sv.length() != 10)
        return "Time::fromMDYString : invalid input " + string(sv);

    if (auto err = fromString(sv.substr(6, 4), t.m_year); !err.empty())
        return "Time::fromMDYString : " + err;

    if (auto err = fromString(sv.substr(0, 2), t.m_month); !err.empty())
        return "Time::fromMDYString : " + err;

    if (auto err = fromString(sv.substr(3, 2), t.m_day); !err.empty())
        return "Time::fromMDYString : " + err;

    t.m_hour = 0;
    t.m_minute = 0;
    t.m_second = 0;

    return "";
}

Error
Time::fromTsString(const string_view& sv, Time& t)
{
    Error error;

    if (sv.length() != 12)
        return "Time::fromTsString : invalid input "s + string(sv);

    if (error = fromString(sv.substr(0, 4), t.m_year); !error.empty())
        return "Time::fromTsString : " + error;

    if (error = fromString(sv.substr(4, 2), t.m_month); !error.empty())
        return "Time::fromTsString : " + error;

    if (error = fromString(sv.substr(6, 2), t.m_day); !error.empty())
        return "Time::fromTsString : " + error;

    if (error = fromString(sv.substr(8, 2), t.m_hour); !error.empty())
        return "Time::fromTsString : " + error;

    if (error = fromString(sv.substr(10, 2), t.m_minute); !error.empty())
        return "Time::fromTsString : " + error;

    return "";
};


string
Time::toDateString() const
{
    string s;

    s += yearToString();
    s += '-';
    s += monthToString();
    s += '-';
    s += dayToString();

    return s;
};

string
Time::toDateTimeString() const
{
    string s;

    s += yearToString();
    s += '-';
    s += monthToString();
    s += '-';
    s += dayToString();
    s += ' ';
    s += hourToString();
    s += ':';
    s += minuteToString();

    return s;
};

string
Time::toIsoString() const
{
    string s;

    s += yearToString();
    s += '-';
    s += monthToString();
    s += '-';
    s += dayToString();
    s += 'T';
    s += hourToString();
    s += ':';
    s += minuteToString();
    s += ':';
    s += secondToString();
    s += 'Z';

    return s;
};

string
Time::toTsDateString() const
{
    string s;

    s += yearToString();
    s += monthToString();
    s += dayToString();

    return s;
};

string
Time::toTsString() const
{
    string s;

    s += yearToString();
    s += monthToString();
    s += dayToString();
    s += hourToString();
    s += minuteToString();

    return s;
};

string
Time::toTsTimeString() const
{
    string s;

    s += hourToString();
    s += minuteToString();

    return s;
};


std::pair<Time, Time>
Time::frameTime(const Duration& dt_) const
{
    if (dt_ == 1_d)
    {
        const auto t = date();

        return {Time(t.year(),t.month(),t.day(), 9,30), Time(t.year(),t.month(),t.day(), 16,0)};
    }

    const auto elapsedSeconds = toJulianDay() - 1;

    auto dt = dt_.seconds();

    Time openTime;
    openTime.fromJulianDay(elapsedSeconds - (elapsedSeconds % dt));

    Time closeTime = openTime + dt_;

    return { openTime, closeTime };
};


bool
Time::isWeekend() const
{
    auto elapsedDays = toJulianDay() / (24 * 60 * 60);

    return (elapsedDays + 2415019) % 7 + 1 > 5;
};


bool
Time::inSameFrame(const Time& t, const Duration& dt) const
{
    const auto elapsedSeconds = t.date().toJulianDay() + 1;

    const auto bucket1 = (  toJulianDay() - elapsedSeconds) / dt.seconds();
    const auto bucket2 = (t.toJulianDay() - elapsedSeconds) / dt.seconds();
    return bucket1 == bucket2;
};


bool
Time::empty() const
{
    return *this == Time();
};


bool
Time::operator<(const Time& rhs) const
{
    return std::tie(m_year, m_month, m_day, m_hour, m_minute, m_second) <
        std::tie(rhs.m_year, rhs.m_month, rhs.m_day, rhs.m_hour, rhs.m_minute, rhs.m_second);
};
bool
Time::operator>(const Time& rhs) const
{
    return rhs < *this;
};

bool
Time::operator<=(const Time& rhs) const
{
    return !(*this > rhs);
};

bool
Time::operator>=(const Time& rhs) const
{
    return !(*this < rhs);
};

bool
Time::operator==(const Time& rhs) const
{
    return std::tie(m_year, m_month, m_day, m_hour, m_minute, m_second) ==
        std::tie(rhs.m_year, rhs.m_month, rhs.m_day, rhs.m_hour, rhs.m_minute, rhs.m_second);
};

bool
Time::operator!=(const Time& rhs) const
{
    return !(*this == rhs);
};



Time
Time::operator+(const Duration& dt) const
{
    auto t = *this;
    t += dt;
    return t;
};


Time&
Time::operator+=(const Duration& dt)
{
    auto elapsedSeconds = toJulianDay();

    switch (dt.m_period)
    {
    case Period_Second:
        elapsedSeconds += dt.m_rep;
        break;
    case Period_Minute:
        elapsedSeconds += dt.m_rep * 60;
        break;
    case Period_Hour:
        elapsedSeconds += dt.m_rep * 60 * 60;
        break;
    case Period_Day:
        elapsedSeconds += dt.m_rep * 60 * 60 * 24;
        break;
    }

    fromJulianDay(elapsedSeconds);

    return *this;
};


Time
Time::operator-(const Duration& dt) const
{
    auto t = *this;
    t -= dt;
    return t;
};


Time&
Time::operator-=(const Duration& dt)
{
    auto elapsedSeconds = toJulianDay();

    switch (dt.m_period)
    {
    case Period_Second:
        elapsedSeconds -= dt.m_rep;
        break;
    case Period_Minute:
        elapsedSeconds -= dt.m_rep * 60;
        break;
    case Period_Hour:
        elapsedSeconds -= dt.m_rep * 60 * 60;
        break;
    case Period_Day:
        elapsedSeconds -= dt.m_rep * 60 * 60 * 24;
        break;
    }

    fromJulianDay(elapsedSeconds);

    return *this;
};




Timer::Timer()
{
    reset();
};

f64
Timer::elapsed_time_sec() const
{
    std::chrono::duration<f64> elapsed = std::chrono::steady_clock::now() - m_origin;

    return elapsed.count();
};

void
Timer::reset()
{
    m_origin = std::chrono::steady_clock::now();
}
