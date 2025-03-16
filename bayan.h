#pragma once
#include <boost/filesystem.hpp>
#include <vector>
#include <string>

std::vector<boost::filesystem::path> get_files(const std::vector<boost::filesystem::path>& directories, const std::vector<boost::filesystem::path>& exclude_dirs, int scan_level, size_t min_file_size, const std::vector<std::string>& masks);
std::vector<std::string> hash_file(const boost::filesystem::path& path, size_t block_size, const std::string& algorithm);
void find_duplicates(const std::vector<boost::filesystem::path>& files, size_t block_size, const std::string& algorithm);
int run(int argc, const char** argv);
