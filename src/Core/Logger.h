#pragma once

#include <iostream>
#include <syncstream>

#include "Core/Core.h"

#undef ERROR
#define NOGDI


#define LOG(level_) \
	((level_) < Logger::level()) ? (void)0 : Voidify() & Logger(level_, __FILE__, __LINE__, __FUNCTION__)

#define LOG2(level_, function_) \
	((level_) < Logger::level()) ? (void)0 : Voidify() & Logger(level_, __FILE__, __LINE__, function_)

namespace tasty
{

enum LoggerLevel {
	DEBUG,
	INFO,
	WARN,
	ERROR
};


/// Inspired by loguru::StreamLogger: https://github.com/emilk/loguru.
///
class Logger
{
private:
	LoggerLevel
		m_level;

	std::osyncstream
		m_cout;

public:
	Logger(LoggerLevel level, const char* file, const u64 line, const char* function);

	~Logger();

	static LoggerLevel& level();


	template<typename T>
	Logger& operator<<(const T& t)
	{
		m_cout << t;
		return *this;
	}

	// std::endl and other iomanip:s.
	Logger& operator<<(std::ostream& (*f)(std::ostream&))
	{
		f(m_cout);
		return *this;
	}
};


class Voidify
{
public:
	Voidify() {}
	// This has to be an operator with a precedence lower than << but higher than ?:
	void operator&(const Logger&) { }
};

}
