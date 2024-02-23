#pragma once

#include <cassert>
#include <cstring>
#include <filesystem>
#include <limits>
#include <memory>
#include <string>
#include <vector>

using namespace std::literals;

namespace fs = std::filesystem;

/// ---- Tracy Profiler --------------------------------------------------------
///
/// Comment this line to ENABLE profiling
///
#undef TRACY_ENABLE


/// Common Types
/// ------------
/// 
using c8 = char;
using f32 = float;
using f64 = double;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

constexpr auto NaN = std::numeric_limits<f64>::quiet_NaN();

using std::string;
using std::string_view;
using std::vector;


namespace tasty
{

/// ---- Error Type ------------------------------------------------------------
///
using Error = std::string;



#ifdef NDEBUG
constexpr bool kDebug = false;
#else
constexpr bool kDebug = true;
#endif


#ifdef __linux__
constexpr bool kLinux = true;
constexpr bool kWindows = false;
#elif _WIN32
constexpr bool kLinux = false;
constexpr bool kWindows = true;
#else
static_assert(false, "Unsupported OS.")
#endif

/// ---- Constants -------------------------------------------------------------
///
constexpr u16 kDaysInYear = 365.;



/// ---- Smart Pointer Types ---------------------------------------------------
///
template<class T>
using Ptr = std::shared_ptr<T>;

template<class T, typename... Args>
Ptr<T> make_ptr(Args &&...args)
{
    return std::make_shared<T>(args...);
};


template<class T>
using sPtr = std::shared_ptr<T>;

template<class T, typename... Args>
sPtr<T> make_sPtr(Args &&...args)
{
    return std::make_shared<T>(args...);
};


template<class T>
using uPtr = std::unique_ptr<T>;

template<class T, typename... Args>
uPtr<T> make_uPtr(Args &&...args)
{
    return std::make_unique<T>(args...);
};


/// String Types
/// ------------
/// 
template<size_t N>
class String
{
private:
    char
        m_buf[N];

public:
    String()
    {
        memset(m_buf, 0, N);
    };

    String(const std::string& src)
    {
        memset(m_buf, 0, N);
        memcpy(m_buf, src.c_str(), std::min(src.size(), N));
    }

    String(const std::string_view& src)
    {
        memset(m_buf, 0, N);
        memcpy(m_buf, src.data(), std::min(src.size(), N));
    }


    friend bool operator< (const String& lhs, const String& rhs)
    {
        return memcmp(lhs.m_buf, rhs.m_buf, N) < 0;
    }
    friend bool operator!= (const String& lhs, const String& rhs)
    {
        return memcmp(lhs.m_buf, rhs.m_buf, N) != 0;
    }

    size_t
        size() const
    {
        size_t i = 0;

        while (i < N && m_buf[i]) { ++i; };

        return i;
    }

    // Implicit conversion to std::string
    operator std::basic_string<char, std::char_traits<char>, std::allocator<char>>() const
    {
        return std::string(m_buf, size());
    }
};

}
