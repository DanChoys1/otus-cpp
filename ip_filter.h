#ifndef IP_FILTER
#define IP_FILTER

#include <set>
#include <vector>
#include <string_view>

std::vector<std::string_view> split(std::string_view str, char d);

std::multiset<uint32_t, std::greater<uint32_t>> parseLogs(std::istream& stream);
uint32_t parseIp(std::string_view ipAddress);
int getIpByte(uint32_t ip, int num);

void printIp(uint32_t ip);
void printIpVec(const std::vector<uint32_t> ips);

std::vector<uint32_t> filter(const std::multiset<uint32_t, std::greater<uint32_t>>& ipPool, int num);
std::vector<uint32_t> filter(const std::multiset<uint32_t, std::greater<uint32_t>>& ipPool, uint32_t num1, uint32_t num2);
std::vector<uint32_t> filter_any(const std::multiset<uint32_t, std::greater<uint32_t>>& ipPool, int num);

#endif