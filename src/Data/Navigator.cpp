#include "Data/Navigator.h"

#include <regex>

#include "Core/Logger.h"

using namespace tasty;



Error
Navigator::ratesPathAll(vector<fs::path>& paths) const
{
    const auto ratesDir = m_rootDir / "rates";

    string mask = "(treasury_rates_[0-9]+\\.csv(\\.zst)?$)";

    if (auto err = listDir(ratesDir, mask, false, paths); !err.empty())
        return "Navigator::snapDirAll : " + err;

    return "";
}



Error
Navigator::listDir(const fs::path& rootDir, const string& mask, bool recursive, vector<fs::path>& paths) const
{
    if (!fs::exists(rootDir))
        return "";

    try {
        std::smatch match;

        auto loop_body = [&](fs::directory_entry src) {
            const auto& srcPath = src.path().string();
            std::regex mask_(mask);
            if (std::regex_search(srcPath, match, mask_)) {
                paths.push_back(src.path());
            }
        };

        if (recursive) {
            for (auto& src : fs::recursive_directory_iterator(rootDir))
                loop_body(src);
        }
        else {
            for (auto& src : fs::directory_iterator(rootDir))
                loop_body(src);
        }

    }
    catch (const fs::filesystem_error& e) {
        auto path = fs::absolute(rootDir);
        return "listDir : path does not exist : rootDir = '" + path.string() + "'";
    }

    return "";
}
