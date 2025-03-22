#include "bayan.h"

#include <boost/filesystem.hpp>
#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <optional>
#include <unordered_set>

using namespace std;
namespace fs = boost::filesystem;

namespace
{

uint32_t hash_block(const vector<char>& block, HashAlgo hashAlgo) 
{
    switch (hashAlgo)
    {
    case HashAlgo::CRC32:
    {
        boost::crc_32_type result;
        result.process_bytes(block.data(), block.size());
        return result.checksum();
    }
    case HashAlgo::MD5:
    {
        boost::uuids::detail::md5 hash;
        boost::uuids::detail::md5::digest_type digest;
        hash.process_bytes(block.data(), block.size());
        hash.get_digest(digest);
        const auto char_digest = reinterpret_cast<const char*>(&digest);
        return string(char_digest, char_digest + sizeof(boost::uuids::detail::md5::digest_type));
    }
    };
    return 0;
}

class MultidimensionalFileSet
{
    class File : public ifstream
    {
    public:
        File(const string& path, ios::openmode ios) :
            ifstream(path, ios), _path(path)
        {}

        string path() const { return _path; }

    private:
        string _path;
    };

public:
    MultidimensionalFileSet() = default;

    MultidimensionalFileSet(size_t blockSize) :
        _blockSize(blockSize)
    {}

    void insert(string path)
    {
        if (insertFirstePath(path))
            return;

        File file(path, ios::binary);
        insert(file);
    }

    vector<vector<string>> files()
    {
        vector<vector<string>> f;
        collectFiles(f);
        return f;
    }

private:
    MultidimensionalFileSet(size_t blockSize, File& file) :
        _blockSize(blockSize)
    {
        if (insertFirstePath(file.path()))
        {
            _filePos = file.tellg();
            return;
        }
        insert(file);
    }

    void insert(File& file)
    {
        if (!file.is_open())
            throw "Не удалось открыть файл: " + file.path();

        if (!_isEof && hasPaths())
        {
            File file2(move(_paths->front()), ios::binary);
            file2.seekg(_filePos);
            _paths.reset();
            _filePos = 0;
            insert(file2);
        }

        vector<char> buffer(_blockSize, 0);
        file.read(buffer.data(), _blockSize);
        if (file.gcount() > 0) 
        {
            uint32_t blockHash = hash_block(buffer, "crc32");

            if (_nextNodes.has_value() && _nextNodes->count(blockHash) != 0)
            {
                MultidimensionalFileSet& node = (*_nextNodes)[blockHash];
                node.insert(file);
                return;
            }

            if (!_nextNodes.has_value())
                _nextNodes.emplace();
            (*_nextNodes)[blockHash] = MultidimensionalFileSet(_blockSize, file);
        }
        else
        {
            _isEof = true;
            setPath(file.path());
        }
    }

    bool insertFirstePath(string path)
    {
        if (hasNextNodes() || hasPaths())
            return false;

        setPath(move(path));
        return true;
    }

    void collectFiles(vector<vector<string>>& files)
    {
        if (hasPaths())
        {
            files.push_back(move(*_paths));
            _paths.reset();
            _isEof = false;
        }
        if (hasNextNodes())
        {
            for (auto& node : *_nextNodes)
            {
                node.second.collectFiles(files);
            }
            _nextNodes.reset();
        }
    }

    bool hasPaths() const 
    {
        return _paths.has_value() && !_paths->empty();
    }

    bool hasNextNodes() const 
    {
        return _nextNodes.has_value() && !_nextNodes->empty();
    }

    void setPath(string path)
    {
        if (!_paths.has_value())
            _paths.emplace({move(path)});
        else
            _paths->emplace_back(move(path));
    }

private:
    size_t _blockSize = 0;
    optional<unordered_map<uint32_t, MultidimensionalFileSet>> _nextNodes;
    optional<vector<string>> _paths;
    bool _isEof = false;
    ifstream::pos_type _filePos = 0;
};

}

vector<vector<string>> getFiles(const vector<fs::path>& directories, const vector<fs::path>& exclude_dirs,
                                    int scan_level, size_t min_file_size, const vector<string>& masks, size_t block_size, HashAlgo hashAlgo) 
{
    MultidimensionalFileSet mFileSet(block_size, hashAlgo);

    for (const auto& dir : directories) 
    {
        if (!fs::exists(dir) || !fs::is_directory(dir)) 
            continue;

        fs::recursive_directory_iterator it(dir);
        fs::recursive_directory_iterator end;
        while (it != end) 
        {
            namespace algo = boost::algorithm;
            if (it.depth() > scan_level || !algo::contains(exclude_dirs, it->path())) 
            {
                it.disable_recursion_pending();
            }
            else if (fs::is_regular_file(*it) && fs::file_size(*it) >= min_file_size) 
            {
                const fs::path& path = it->path();
                bool isMatch = algo::any_of(masks, bind(algo::iends_with<string, string>, 
                                                                path.filename().string(), 
                                                                placeholders::_1));
                if (masks.empty() || isMatch)
                    mFileSet.insert(path.string());
            }

            ++it;
        }
    }
    
    return mFileSet.files();
}