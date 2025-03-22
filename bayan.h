#pragma once
#include <boost/filesystem.hpp>
#include <vector>
#include <string>

namespace fs = boost::filesystem;

class HashAlgoConverter
{
public:
    enum class HashAlgo
    {
        CRC32,
        MD5,
    };

    static HashAlgo fromStr(const std::string& hashName)
    {
        return hashAlgoMap[hashName];
    }

    static bool isContains(const std::string& hashName)
    {
        return hashAlgoMap.count(hashName) != 0;
    }

private:
    inline static std::unordered_map<std::string, HashAlgo> hashAlgoMap
    {
        { "crc32", HashAlgo::CRC32 },
        { "md5", HashAlgo::MD5 },
    };
};
using HashAlgo = HashAlgoConverter::HashAlgo;

std::vector<std::vector<std::string>> getFiles(const std::vector<fs::path>& directories, 
                                const std::vector<fs::path>& exclude_dirs,
                                int scan_level, 
                                size_t min_file_size, 
                                const std::vector<std::string>& masks, 
                                size_t block_size, 
                                HashAlgo hashAlgo);
