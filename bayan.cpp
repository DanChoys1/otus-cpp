#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/crc.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <optional>
#include <unordered_set>
#include "bayan.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

uint32_t hash_block(const std::vector<char>& block, const std::string& algorithm) 
{
    if (algorithm == "crc32")
    {
        boost::crc_32_type result;
        result.process_bytes(block.data(), block.size());
        return result.checksum();
    } 
    // else if (algorithm == "md5") 
    // {
    //     boost::uuids::detail::md5 hash;
    //     boost::uuids::detail::md5::digest_type digest;
    //     hash.process_bytes(block.data(), block.size());
    //     hash.get_digest(digest);
    //     const auto char_digest = reinterpret_cast<const char*>(&digest);
    //     return std::string(char_digest, char_digest + sizeof(boost::uuids::detail::md5::digest_type));
    // }
    // Add other hashing algorithms if needed
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

    MultidimensionalFileSet(std::size_t blockSize) :
        _blockSize(blockSize)
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
    MultidimensionalFileSet(std::size_t blockSize, File& file) :
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
            _paths.emplace({std::move(path)});
        else
            _paths->emplace_back(std::move(path));
    }

private:
    std::size_t _blockSize = 0;
    std::optional<std::unordered_map<uint32_t, MultidimensionalFileSet>> _nextNodes;
    std::optional<std::vector<std::string>> _paths;
    bool _isEof = false;
    std::ifstream::pos_type _filePos = 0;
};

// std::vector<std::string> 
std::vector<std::vector<std::string>> getFiles(const std::vector<fs::path>& directories, const std::vector<fs::path>& exclude_dirs,
                                    int scan_level, size_t min_file_size, const std::vector<std::string>& masks) 
{
    // std::vector<std::string> files;
    MultidimensionalFileSet mFileSet(5);

    for (const auto& dir : directories) 
    {
        if (!fs::exists(dir) || !fs::is_directory(dir)) 
            continue;

        fs::recursive_directory_iterator it(dir);
        fs::recursive_directory_iterator end;
        while (it != end) 
        {
            std::string p = it->path().string();
            if (it.depth() > scan_level) 
            {
                it.disable_recursion_pending();
            }
            else if (std::find(exclude_dirs.begin(), exclude_dirs.end(), it->path()) != exclude_dirs.end()) 
            {
                it.disable_recursion_pending();
            }
            else if (fs::is_regular_file(*it) && fs::file_size(*it) >= min_file_size) 
            {
                bool match = masks.empty() || std::any_of(masks.cbegin(), masks.cend() , [name = it->path().filename().string()](const std::string& mask)
                {
                    return boost::algorithm::iends_with(name, mask);
                });

                if (match) 
                    mFileSet.insert(it->path().string());
                    // files.push_back(it->path());
            }

            ++it;
        }
    }
    // return files;
    return mFileSet.files();
}

int run(int argc, const char** argv)
{
    // Добавляем пункты меню
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("directories,d", po::value<std::vector<fs::path>>()->multitoken(), "directories to scan")
        ("exclude,e", po::value<std::vector<fs::path>>()->multitoken(), "directories to exclude from scanning")
        ("level,l", po::value<int>()->default_value(0), "scan level (0 - only specified directory)")
        ("min-size,m", po::value<size_t>()->default_value(1), "minimum file size")
        ("masks,k", po::value<std::vector<std::string>>()->multitoken(), "file name masks")
        ("block-size,b", po::value<size_t>()->default_value(1024), "block size for reading files")
        ("algorithm,a", po::value<std::string>()->default_value("crc32"), "hashing algorithm (crc32, md5)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm); 

    if (vm.count("help")) 
    {
        std::cout << desc << std::endl;
        return 1;
    }

    try {
        // auto directories = vm["directories"].as<std::vector<fs::path>>();
        // auto exclude_dirs = vm["exclude"].as<std::vector<fs::path>>();
        // int scan_level = vm["level"].as<int>();
        // size_t min_file_size = vm["min-size"].as<size_t>();
        // auto masks = vm["masks"].as<std::vector<std::string>>();
        // size_t block_size = vm["block-size"].as<size_t>();
        // std::string algorithm = vm["algorithm"].as<std::string>();

        auto files = getFiles({".\\123"}, {}, 100, 1, {});
        for (const auto& file : files)
        {
            for (const auto& f : file)
                std::cout << f << std::endl;
            std::cout << std::endl;
        }
        // find_duplicates(files, block_size, algorithm);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
