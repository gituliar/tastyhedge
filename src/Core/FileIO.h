#pragma once

#include <fstream>

#include "Core/Core.h"


namespace tasty
{

class TextFileReader
{
private:
    string
        m_buf;
    std::ifstream
        m_src;


public:
    ~TextFileReader();

    Error
        close();
    Error
        open(const fs::path& srcPath);
    bool
        readline(string_view& dst);
};



class TextFileWriter
{
private:
    std::ofstream
        m_dst;

public:
    ~TextFileWriter();

    Error
        close();
    Error
        open(const string& dstPath);
    Error
        write(const string& src);
};

}
