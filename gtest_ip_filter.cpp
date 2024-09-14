#include "ip_filter.h"
#include "gtest_data.h"

#include <gtest/gtest.h>

// filter by first byte //
TEST(gtest_ip_filter, test_fist_byte_filter)
{
    static const std::vector<uint32_t> result
    {
        parseIp("1.231.69.33"),
        parseIp("1.87.203.225"),
        parseIp("1.70.44.170"),
        parseIp("1.29.168.152"),
        parseIp("1.1.234.8"),
    };

    std::stringstream logsStream(GTestData::IpLog);
    std::vector<uint32_t> filterIps = filter(parseLogs(logsStream), 1);

    ASSERT_EQ(filterIps.size(), result.size()) << "Vectors filterIps and result are of unequal length " << std::endl;
    for (size_t i = 0; i < filterIps.size(); ++i)
        EXPECT_EQ(filterIps[i], result[i]) << "Vectors filterIps and result differ at index " << i;
}

// filter by first and second bytes //
TEST(gtest_ip_filter, test_fist_second_byte_filter)
{
    static const std::vector<uint32_t> result
    {
        parseIp("46.70.225.39"),
        parseIp("46.70.147.26"),
        parseIp("46.70.113.73"),
        parseIp("46.70.29.76"),
    };

    std::stringstream logsStream(GTestData::IpLog);
    std::vector<uint32_t> filterIps = filter(parseLogs(logsStream), 46, 70);

    ASSERT_EQ(filterIps.size(), result.size()) << "Vectors filterIps and result are of unequal length " << std::endl;
    for (size_t i = 0; i < filterIps.size(); ++i)
        EXPECT_EQ(filterIps[i], result[i]) << "Vectors filterIps and result differ at index " << i;
}

// filter by any byte //
TEST(gtest_ip_filter, test_any_byte_filter)
{
    static const std::vector<uint32_t> result
    {
        parseIp("186.204.34.46"),
        parseIp("186.46.222.194"),
        parseIp("185.46.87.231"),
        parseIp("185.46.86.132"),
        parseIp("185.46.86.131"),
        parseIp("185.46.86.131"),
        parseIp("185.46.86.22"),
        parseIp("185.46.85.204"),
        parseIp("185.46.85.78"),
        parseIp("68.46.218.208"),
        parseIp("46.251.197.23"),
        parseIp("46.223.254.56"),
        parseIp("46.223.254.56"),
        parseIp("46.182.19.219"),
        parseIp("46.161.63.66"),
        parseIp("46.161.61.51"),
        parseIp("46.161.60.92"),
        parseIp("46.161.60.35"),
        parseIp("46.161.58.202"),
        parseIp("46.161.56.241"),
        parseIp("46.161.56.203"),
        parseIp("46.161.56.174"),
        parseIp("46.161.56.106"),
        parseIp("46.161.56.106"),
        parseIp("46.101.163.119"),
        parseIp("46.101.127.145"),
        parseIp("46.70.225.39"),
        parseIp("46.70.147.26"),
        parseIp("46.70.113.73"),
        parseIp("46.70.29.76"),
        parseIp("46.55.46.98"),
        parseIp("46.49.43.85"),
        parseIp("39.46.86.85"),
        parseIp("5.189.203.46")
    };

    std::stringstream logsStream(GTestData::IpLog);
    std::vector<uint32_t> filterIps = filter_any(parseLogs(logsStream), 46);

    ASSERT_EQ(filterIps.size(), result.size()) << "Vectors filterIps and result are of unequal length " << std::endl;
    for (size_t i = 0; i < filterIps.size(); ++i)
        EXPECT_EQ(filterIps[i], result[i]) << "Vectors filterIps and result differ at index " << i;
}