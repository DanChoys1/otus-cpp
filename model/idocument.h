#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include <memory>
#include <string>

#include "iprimitive.h"

using DocumentData = std::string;

class IDocument
{
public:
    virtual ~IDocument() = default;

    virtual void setPrimitive(std::unique_ptr<IPrimitive> item) = 0;    
    virtual void removePrimitive(int index) = 0;
    virtual void clear() = 0;

    virtual DocumentData data() const = 0;

};

#endif