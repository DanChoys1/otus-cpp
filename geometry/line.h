#ifndef LINE_H 
#define LINE_H 

#include "point.h"
#include "custom_to_string.h"

struct Line
{
    Point p1;
    Point p2;
};

std::ostream& operator<< (std::ostream& str, const Line& l)
{
    return str << "Line{ p1 = " << to_string(l.p1) << ", p2 = " << to_string(l.p2) << " }";
}

#endif