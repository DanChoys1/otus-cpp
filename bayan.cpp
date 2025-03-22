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

namespace
{

std::string hashBlock(const std::vector<char>& block, HashAlgo hashAlgo) 
{
    switch (hashAlgo)
    {
    case HashAlgo::CRC32:
    {
        boost::crc_32_type result;
        result.process_bytes(block.data(), block.size());
        uint32_t digest = result.checksum();
        const auto char_digest = reinterpret_cast<const char*>(&digest);
        return std::string(char_digest, char_digest + sizeof(digest));
    }
    case HashAlgo::MD5:
    {
        boost::uuids::detail::md5 hash;
        boost::uuids::detail::md5::digest_type digest;
        hash.process_bytes(block.data(), block.size());
        hash.get_digest(digest);
        const auto char_digest = reinterpret_cast<const char*>(&digest);
        return std::string(char_digest, char_digest + sizeof(boost::uuids::detail::md5::digest_type));
    }
    };
    return 0;
}

class MultidimensionalFileSet
{
    class File : public std::ifstream
    {
    public:
        File(const std::string& path, std::ios::openmode ios) :
        std::ifstream(path, ios), _path(path)
        {}

        std::string path() const { return _path; }

    private:
        std::string _path;
    };

public:
    MultidimensionalFileSet() = default;

    MultidimensionalFileSet(size_t blockSize, HashAlgo hashAlgo) :
        _blockSize(blockSize), _hashAlgo(hashAlgo)
    {}

    void insert(std::string path)
    {
        if (insertFirstePath(path))
            return;

        File file(path, std::ios::binary);
        insert(file);
    }

    std::vector<std::vector<std::string>> files()
    {
        std::vector<std::vector<std::string>> f;
        collectFiles(f);
        return f;
    }

private:
    MultidimensionalFileSet(size_t blockSize, HashAlgo hashAlgo, File& file) :
        _blockSize(blockSize), _hashAlgo(hashAlgo)
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
            File file2(std::move(_paths->front()), std::ios::binary);
            file2.seekg(_filePos);
            _paths.reset();
            _filePos = 0;
            insert(file2);
        }

        std::vector<char> buffer(_blockSize, 0);
        file.read(buffer.data(), _blockSize);
        if (file.gcount() > 0) 
        {
            std::string blockHash = hashBlock(buffer, _hashAlgo);

            if (_nextNodes.has_value() && _nextNodes->count(blockHash) != 0)
            {
                MultidimensionalFileSet& node = (*_nextNodes)[blockHash];
                node.insert(file);
                return;
            }

            if (!_nextNodes.has_value())
                _nextNodes.emplace();
            (*_nextNodes)[blockHash] = MultidimensionalFileSet(_blockSize, _hashAlgo, file);
        }
        else
        {
            _isEof = true;
            setPath(file.path());
        }
    }

    bool insertFirstePath(std::string path)
    {
        if (hasNextNodes() || hasPaths())
            return false;

        setPath(std::move(path));
        return true;
    }

    void collectFiles(std::vector<std::vector<std::string>>& files)
    {
        if (hasPaths())
        {
            files.push_back(std::move(*_paths));
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

    void setPath(std::string path)
    {
        if (!_paths.has_value())
            _paths.emplace({move(path)});
        else
            _paths->emplace_back(move(path));
    }

private:
    size_t _blockSize = 0;
    HashAlgo _hashAlgo = HashAlgo::CRC32;
    std::optional<std::unordered_map<std::string, MultidimensionalFileSet>> _nextNodes;
    std::optional<std::vector<std::string>> _paths;
    bool _isEof = false;
    std::ifstream::pos_type _filePos = 0;
};

}

std::vector<std::vector<std::string>> getFiles(const std::vector<fs::path>& directories, const std::vector<fs::path>& exclude_dirs,
                                    int scan_level, size_t min_file_size, const std::vector<std::string>& masks, size_t block_size, HashAlgo hashAlgo) 
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
            if (it.depth() > scan_level || algo::contains(exclude_dirs, it->path())) 
            {
                it.disable_recursion_pending();
            }
            else if (fs::is_regular_file(*it) && fs::file_size(*it) >= min_file_size) 
            {
                const fs::path& path = it->path();
                bool isMatch = algo::any_of(masks, [name = path.filename().string()](const std::string& mask)
                {
                    return algo::iends_with(name, mask);
                });
                if (masks.empty() || isMatch)
                    mFileSet.insert(path.string());
            }

            ++it;
        }
    }
    
    return mFileSet.files();
}