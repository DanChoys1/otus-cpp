#ifndef IPRIMITIVE_H 
#define IPRIMITIVE_H 

#include <string>

using PrimitiveData = std::string;

class IPrimitive
{
public:
    virtual ~IPrimitive() = default;
    virtual PrimitiveData data() const = 0;
};

#endif