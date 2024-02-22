#include "Core/Csv.h"

#include "Core/String.h"
#include "Core/Time.h"

using namespace tasty;


/// Helpers
///
template<typename T, typename V>
i64 findIndex(const T& src, const V& value) {
    if (auto ii = std::find(src.begin(), src.end(), value); ii == src.end())
        return -1;
    else
        return std::distance(src.begin(), ii);
}


void
join(const std::vector<std::string>& src, char sep, std::string& dst)
{
    if (src.empty())
        return;

    dst += src[0];
    for (auto i = 1; i < src.size(); ++i)
    {
        dst += sep;
        dst += src[i];
    }
};


void
split(const string_view& src, vector<string_view>& dst, char sep = ',', const u32 maxSplit = 0)
{
    dst.clear();

    const char* cur = src.data();
    const char* end = cur + src.size();

    const char* prev = cur;
    while ((cur = (char*)std::memchr(cur, sep, end - cur)) != nullptr && (dst.size() < maxSplit || maxSplit == 0))
    {
        dst.push_back(src.substr(prev - src.data(), cur - prev));
        prev = ++cur;
    };

    if (dst.size() < maxSplit || maxSplit == 0)
        dst.push_back(src.substr(prev - src.data(), end - prev));
}


/// Definition
///
Error
CsvReader::close()
{
    return m_src->close();
}


Error
CsvReader::get(const kCsv_ id, Time& val) const
{
    std::string_view val_;
    if (auto err = get(id, val_); !err.empty())
        return err;

    if (auto err = Time::fromTsString(val_, val); !err.empty())
        return "CsvReader::get : " + err;

    return "";
}

Error
CsvReader::getMDY(const kCsv_ id, Time& val) const
{
    std::string_view val_;
    if (auto err = get(id, val_); !err.empty())
        return err;

    if (auto err = Time::fromMDYString(val_, val); !err.empty())
        return "CsvReader::get : " + err;

    return "";
}



Error
CsvReader::open(const fs::path& path_)
{
    LOG(DEBUG) << "OPEN " << fs::absolute(path_);

    const auto& path = path_.string();

    if (path.ends_with("zst")) {
        //m_src = make_ptr<ZstdFileReader>();
    }
    else {
        m_src = make_ptr<TextFileReader>();
    }

    if (auto err = m_src->open(path); !err.empty())
        return "CsvReader::open : " + err;

    if (auto err = m_src->readline(m_line); !err.empty())
        return "CsvReader::open : " + err;

    /// split header
    ///
    split(m_line, m_buf);
    for (auto i = 0; i < kCsv_Size; i++)
        m_col[i] = findIndex(m_buf, kCsvNames[i]);

    m_header = m_line;

    return "";
}


Error
CsvReader::readline()
{
    if (auto err = m_src->readline(m_line); !err.empty())
        return "CsvReader::readline : " + err;

    split(m_line, m_buf);

    return "";
}


Error
CsvWriter::close()
{
    if (Error err; !m_dst->close(err))
        return "CsvWriter::close : " + err;

    return "";
}


Error
CsvWriter::open(const fs::path& dstPath)
{
    if (dstPath.string().ends_with(".zst") || dstPath.string().ends_with(".zst.buf")) {
        //m_dst = make_ptr<ZstdFileWriter>();
    }
    else {
        m_dst = make_ptr<TextFileWriter>();
    }

    if (Error err; !m_dst->open(dstPath.string(), err))
        return err;

    return "";
}


Error
CsvWriter::writeline(const std::string_view& line)
{
    Error err;
    //  FIXME: switch to std::string_view
    if (!m_dst->write(std::string(line), err))
        return "CsvWriter::writeline : " + err;
    if (!m_dst->write("\n", err))
        return "CsvWriter::writeline : " + err;

    return "";
}