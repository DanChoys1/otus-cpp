#include "ip_filter.h"

#include <iostream>
#include <functional>
#include <sstream>

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