#ifndef RECT_PRIMITIVE_H 
#define RECT_PRIMITIVE_H 

#include "iprimitive.h"
#include "rect.h"

class RectPrimitive : public IPrimitive
{
public:
    RectPrimitive(Rect rect) :
        _rect{ rect } {}

    virtual PrimitiveData data() const override
    {
        return to_string(_rect);
    }

private:
    Rect _rect;
    
};

#endif