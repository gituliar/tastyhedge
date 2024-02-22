#pragma once

#include "Core/Time.h"


namespace tasty
{

class Navigator
{
private:
    fs::path
        m_rootDir;

public:
    Navigator() : Navigator("c:/mds") {};

    Navigator(const fs::path& rootDir) :
        m_rootDir { fs::absolute(rootDir) }
    {};


    const fs::path&
        rootDir() const { return m_rootDir; };

    Error
        ratesPathAll(vector<fs::path>& paths) const;


private:
    Error
        listDir(const fs::path& rootDir, const string& mask, bool recursive, vector<fs::path>& paths) const;
};

}
