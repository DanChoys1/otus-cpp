#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <memory>
#include <vector>

#include "idocument.h"

class Document : public IDocument
{
public:
    virtual void setPrimitive(std::unique_ptr<IPrimitive> item) override
    {
        _prims.push_back(std::move(item));
    }

    virtual void removePrimitive(int index) override
    {
        _prims.erase(_prims.begin() + index);
    }

    virtual void clear() override
    {
        _prims.clear();
    }

    virtual DocumentData data() const override
    {
        DocumentData d = "";
        for(const auto& pr : _prims)
            d += pr->data() + '\n';
        
        return d;
    }

protected:
    std::vector<std::unique_ptr<IPrimitive>> _prims;

};

#endif