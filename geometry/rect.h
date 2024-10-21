#ifndef RECT_H 
#define RECT_H 

#include "point.h"
#include "custom_to_string.h"

struct Rect
{
    Point p1; // Верхний левый угол
    Point p2; // Нижний правый угол
};

std::ostream& operator<< (std::ostream& str, const Rect& l)
{
    return str << "Rect{ p1 = " << to_string(l.p1) << ", p2 = " << to_string(l.p2) << " }";
}

#endif