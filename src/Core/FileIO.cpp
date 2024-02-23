#include "FileIO.h"

#include <fstream>

#include "Core/Logger.h"

using namespace tasty;



TextFileReader::~TextFileReader()
{
    if (auto err = close(); !err.empty())
        LOG(ERROR) << err;
}

Error
TextFileReader::close()
{
    m_src.close();

    return "";
};

Error
TextFileReader::open(const fs::path& srcPath)
{
    m_src.open(srcPath);
    if (!m_src.is_open())
        return "TextFileReader::open : Fail to open " + srcPath.string();

    return "";
};

bool
TextFileReader::readline(std::string_view& dst)
{
    auto isOpen = bool(std::getline(m_src, m_buf));

    dst = m_buf;

    return isOpen;
};



TextFileWriter::~TextFileWriter()
{
    if (auto err = close(); !err.empty())
        LOG(ERROR) << err;
}

Error
TextFileWriter::close()
{
    if (m_dst.is_open()) {
        m_dst.close();
    }

    return "";
};


Error
TextFileWriter::open(const std::string& path)
{
    fs::create_directories(fs::path(path).parent_path());

    m_dst.open(path, std::ios::binary);

    return "";
};


Error
TextFileWriter::write(const std::string& src)
{
    m_dst << src;

    return "";
};
