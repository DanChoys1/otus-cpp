#include <iostream>
#include <boost/program_options.hpp>
#include "bayan.h"

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, const char** argv) 
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("directories,d", po::value<vector<fs::path>>()->multitoken()->default_value({"."}, ""), "directories to scan")
        ("exclude,e", po::value<vector<fs::path>>()->multitoken()->default_value({}, ""), "directories to exclude from scanning")
        ("level,l", po::value<int>()->default_value(0), "scan level (0 - only specified directory)")
        ("min-size,m", po::value<size_t>()->default_value(1), "minimum file size")
        ("masks,k", po::value<vector<string>>()->multitoken()->default_value({}, ""), "file name masks")
        ("block-size,b", po::value<size_t>()->default_value(5), "block size for reading files")
        ("algorithm,a", po::value<string>()->default_value("crc32"), "hashing algorithm (crc32, md5)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm); 

    if (vm.count("help")) 
    {
        cout << desc << endl;
        return 0;
    }

    try 
    {
        string algorithm = vm["algorithm"].as<string>();
        if (!HashAlgoConverter::isContains(algorithm))
            throw "Такого алгоритма нет";

        auto files = getFiles(vm["directories"].as<vector<fs::path>>(),
                                vm["exclude"].as<vector<fs::path>>(), 
                                vm["level"].as<int>(), 
                                vm["min-size"].as<size_t>(), 
                                vm["masks"].as<vector<string>>(),
                                vm["block-size"].as<size_t>(), 
                                HashAlgoConverter::fromStr(algorithm));
        for (const auto& file : files)
        {
            for (const auto& f : file)
                cout << f << endl;
            cout << endl;
        }
    } 
    catch (const exception& e) 
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
