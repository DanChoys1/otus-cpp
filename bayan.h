#pragma once
#include <boost/filesystem.hpp>
#include <vector>
#include <string>

class HashAlgoConverter
{
public:
    enum class HashAlgo
    {
        CRC32,
        MD5,
    };

    static HashAlgo fromStr(const string& hashName)
    {
        return hashAlgoMap[hashName];
    }

    static bool isContains(const string& hashName)
    {
        hashAlgoMap.count(hashName) != 0;
    }

private:
    inline static unordered_map<string, HashAlgo> hashAlgoMap
    {
        { "crc32", HashAlgo::CRC32 },
        { "md5", HashAlgo::MD5 },
    };
};
using HashAlgo = HashAlgoConverter::HashAlgo;

vector<vector<string>> getFiles(const vector<fs::path>& directories, 
                                const vector<fs::path>& exclude_dirs,
                                int scan_level, 
                                size_t min_file_size, 
                                const vector<string>& masks, 
                                size_t block_size, 
                                HashAlgo hashAlgo);
