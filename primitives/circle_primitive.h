#ifndef CIRCLE_PRIMITIVE_H 
#define CIRCLE_PRIMITIVE_H 

#include "iprimitive.h"
#include "circle.h"
 
class CirclePrimitive : public IPrimitive
{
public:
    CirclePrimitive(Circle circle) :
        _circle{ circle } {}

    virtual PrimitiveData data() const override
    {
        return to_string(_circle);
    }

private:
    Circle _circle;
    
};

#endif