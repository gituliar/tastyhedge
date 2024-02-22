#pragma once

#include <fstream>

#include "Core/Core.h"


namespace tasty
{

class FileReader
{
public:
    virtual bool
        eof() const = 0;

    virtual Error
        close() = 0;
    virtual Error
        open(const fs::path& srcPath) = 0;
    virtual Error
        read(string_view& dst) = 0;
    virtual Error
        readline(string_view& dst, bool ignore_newline = true) = 0;
};


class FileWriter
{
public:
    virtual bool
        close(string& error) = 0;
    virtual bool
        open(const string& path, string& error) = 0;
    virtual bool
        write(const string& src, string& error) = 0;
};



class TextFileReader : public FileReader
{
private:
    fs::path
        m_path;

    FILE*
        m_src;

    c8* m_buf;
    u64 m_capacity;
    u64 m_size;

    u32 m_cursor;


public:
    TextFileReader();
    ~TextFileReader();

    bool
        eof() const override;

    Error
        close() override;
    Error
        open(const fs::path& srcPath) override;
    Error
        read(string_view& dst) override;
    Error
        readline(string_view& dst, bool ignore_newline = true) override;
};



class TextFileWriter : public FileWriter
{
private:
    std::ofstream
        m_dst;

public:
    // FileWriter

    ~TextFileWriter();

    bool
        close(string& error) override;
    bool
        open(const string& path, string& error) override;
    bool
        write(const string& src, string& error) override;
};

}
