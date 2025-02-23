#ifndef UUID_GENERATOR_H
#define UUID_GENERATOR_H

#include <random>
#include <sstream>
#include <iomanip>
#include <string>

// Можно использовать boost::uuid но пока проблемы с подключением буста
std::string generateUUID()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i < 8; i++) 
        ss << dis(gen);
    ss << "-";
    for (int i = 0; i < 4; i++) 
        ss << dis(gen);
    ss << "-4";
    for (int i = 0; i < 3; i++) 
        ss << dis(gen);
    ss << "-";
    ss << dis2(gen);
    for (int i = 0; i < 3; i++) 
        ss << dis(gen);
    ss << "-";
    for (int i = 0; i < 12; i++) 
        ss << dis(gen);
    return ss.str();
}

#endif
