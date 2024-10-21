#ifndef CIRCLE_H 
#define CIRCLE_H 

#include "point.h"
#include "custom_to_string.h"

struct Circle
{
    Point p;
    int r = 0;
};

std::ostream& operator<< (std::ostream& str, const Circle& c)
{
    return str << "Circle{ p = " << to_string(c.p) << ", r = " << to_string(c.r) << " }";
}

#endif