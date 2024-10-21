#ifndef LINE_PRIMITIVE_H 
#define LINE_PRIMITIVE_H 

#include "iprimitive.h"
#include "line.h"

class LinePrimitive : public IPrimitive
{
public:
    LinePrimitive(Line line) :
        _line{ line } {}

    virtual PrimitiveData data() const override
    {
        return to_string(_line);
    }

private:
    Line _line;
    
};

#endif