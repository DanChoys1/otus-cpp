#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <string_view>
#include <functional>
#include <sstream>
#include <utility>

std::vector<std::string_view> split(std::string_view str, char d);
uint32_t parseIp(std::string_view ipAddress);

std::multiset<uint32_t, std::greater<uint32_t>> parseLogs(std::istream& stream)
{
    std::multiset<uint32_t, std::greater<uint32_t>> ipPool;
    std::string ipStr = "";
    while(std::getline(stream, ipStr))
    {
        if (ipStr.empty()) 
            break; 
        
        ipPool.emplace(parseIp(split(ipStr,' ').at(0)));
    }

    return ipPool;
}

std::vector<std::string_view> split(std::string_view str, char d)
{
    std::vector<std::string_view> r;

    std::string_view::size_type start = 0;
    std::string_view::size_type stop = str.find_first_of(d);
    while(stop != std::string_view::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

uint32_t parseIp(std::string_view ipAddress) 
{
  uint ipBytes[4];
  sscanf(ipAddress.data(), "%u.%u.%u.%u", &ipBytes[3], &ipBytes[2], &ipBytes[1], &ipBytes[0]);
  return ipBytes[0] | ipBytes[1] << 8 | ipBytes[2] << 16 | ipBytes[3] << 24;
}

int getIpByte(uint32_t ip, int num)
{
    return (ip >> 8*num) & 255;
}

void printIp(uint32_t ip)
{
    std::cout << getIpByte(ip, 3) << "." << getIpByte(ip, 2) << "." << getIpByte(ip, 1) << "." << getIpByte(ip, 0) << std::endl;
}

void printIpVec(const std::vector<uint32_t> ips)
{
    for(auto ip : ips)
    {
        printIp(ip);
    }
}

std::vector<uint32_t> filter(const std::multiset<uint32_t, std::greater<uint32_t>>& ipPool, int num)
{
    std::vector<uint32_t> filterNums;
    uint32_t ipStart = num << 24;
    uint32_t ipEnd = (num+1) << 24;
    std::copy(ipPool.lower_bound(ipEnd), ipPool.lower_bound(ipStart), std::back_inserter(filterNums));

    return filterNums;
}

std::vector<uint32_t> filter(const std::multiset<uint32_t, std::greater<uint32_t>>& ipPool, uint32_t num1, uint32_t num2)
{
    std::vector<uint32_t> filterNums;
    uint32_t ipStart = num1 << 24 | num2 << 16;
    if (num2 < 255)
        ++num2;
    else
        ++num1;
    uint32_t ipEnd = num1 << 24 | num2 << 16;

    std::copy(ipPool.lower_bound(ipEnd), ipPool.lower_bound(ipStart), std::back_inserter(filterNums));

    return filterNums;
};

std::vector<uint32_t> filter_any(const std::multiset<uint32_t, std::greater<uint32_t>>& ipPool, int num)
{
    std::vector<uint32_t> filterNums;
    for(auto ip : ipPool)
    {
        if (getIpByte(ip, 3) == num || getIpByte(ip, 2) == num ||
                getIpByte(ip, 1) == num || getIpByte(ip, 0) == num)
        {
            filterNums.push_back(ip);
        }
    }

    return filterNums;
}

int main()
{
    try
    {
        //reverse lexicographically sort multiset
        std::multiset<uint32_t, std::greater<uint32_t>> ipPool = parseLogs(std::cin);

        // print reverse lexicographically sort multiset //
        for(auto ip : ipPool)
        {
            printIp(ip);
        }
        // RESULT:
        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // filter by first byte and output //
        std::vector<uint32_t> filterIps = filter(ipPool, 1);
        printIpVec(filterIps);
        // RESULT:
        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // filter by first and second bytes and output //
        filterIps = filter(ipPool, 46, 70);
        printIpVec(filterIps);
        // RESULT:
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // filter by any byte and output //
        filterIps = filter_any(ipPool, 46);
        printIpVec(filterIps);
        // RESULT:
        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
