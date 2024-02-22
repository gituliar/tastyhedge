#include "FileIO.h"

#include <cstdio>

#include "Core/Logger.h"

using namespace tasty;


TextFileReader::TextFileReader() :
    m_buf{ nullptr }, m_capacity{ 4096 }, m_cursor{ 0 }, m_size{ 0 }, m_src { nullptr }
{ };


TextFileReader::~TextFileReader()
{
    if (m_buf)
        std::free(m_buf);

    if (auto err = close(); !err.empty())
        LOG(ERROR) << err;
}

bool
TextFileReader::eof() const
{
    return m_cursor == m_size && std::feof(m_src);
};

Error
TextFileReader::close()
{
    if (!m_src)
        return "";

    if (auto res = std::fclose(m_src); res == EOF)
        return "TextFileReader::close : Fail to close : errno = " + std::to_string(res);

    m_src = nullptr;

    return "";
};

Error
TextFileReader::open(const fs::path& srcPath)
{
    m_path = srcPath;

    if (!m_buf) {
        m_buf = (c8 *)std::malloc(m_capacity);
        if (!m_buf)
            return "TextFileReader::open : Fail to malloc " + std::to_string(m_capacity) + " bytes";
    }

    m_src = std::fopen(srcPath.string().c_str(), "rb");
    if (!m_src)
        return "TextFileReader::open : Fail to open " + srcPath.string();

    return "";
};

Error
TextFileReader::read(std::string_view& dst)
{
    if (m_size = std::fread(m_buf + m_cursor, 1, m_capacity - m_cursor, m_src); !m_size) {
        if (std::ferror(m_src))
            return "TextFileReader::read : Fail to read : errno = " + std::to_string(m_size);
    }

    m_cursor = m_size;

    dst = std::string_view(m_buf, m_size);

    return "";
};

Error
TextFileReader::readline(std::string_view& dst, bool ignore_newline)
{
    do {
        if (const auto p = (c8*)memchr(m_buf + m_cursor, '\n', m_size - m_cursor); p) {
            dst = std::string_view(m_buf + m_cursor, p - (m_buf + m_cursor) + 1 - i8(ignore_newline));
            m_cursor = p - m_buf + 1;
            return "";
        }

        /// Read last line (has no \n)
        ///
        if (std::feof(m_src)) {
            dst = std::string_view(m_buf + m_cursor, m_size - m_cursor);
            m_cursor = m_size;
            return "";
        }

        /// Copy tail to the beginning
        ///
        if (m_cursor < m_size) {
            auto n = m_size - m_cursor;
            std::memcpy(m_buf, m_buf + m_cursor, n);
        }

        /// Consume more data
        ///
        std::string_view buf;
        if (auto err = read(buf); err != "")
            return "TextFileReader::readline : " + err;
        m_cursor = 0;
    } while (m_size);

    return "TextFileReader::readline : EOF";
};



TextFileWriter::~TextFileWriter()
{
    if (Error err; !close(err))
        LOG(ERROR) << err;
}

bool
TextFileWriter::close(std::string& error)
{
    if (m_dst.is_open()) {
        m_dst.close();
    }

    return true;
};


bool
TextFileWriter::open(const std::string& path, std::string& error)
{
    fs::create_directories(fs::path(path).parent_path());

    m_dst.open(path, std::ios::binary);

    return true;
};


bool
TextFileWriter::write(const std::string& src, std::string& error)
{
    m_dst << src;

    return true;
};
