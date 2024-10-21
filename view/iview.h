#ifndef IVIEW_H
#define IVIEW_H

#include "idocument.h"

class IView
{
public:
    virtual ~IView() = default;

    virtual void display(const IDocument& doc) = 0;

    virtual bool showDialog(const std::string& msg) = 0;
    
};

#endif