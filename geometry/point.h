#ifndef POINT_H 
#define POINT_H 

#include <ostream>

struct Point
{
    int x = 0;
    int y = 0;
};

std::ostream& operator<< (std::ostream& str, const Point& p)
{
    return str << "Point{ x = " << std::to_string(p.x) << ", y = " << std::to_string(p.y) << " }";
}

#endif