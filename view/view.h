#ifndef VIEW_H
#define VIEW_H

#include <iostream>

#include "iview.h"
#include "idocument.h"

class View : public IView
{
public:
    virtual void display(const IDocument& doc) override
    {
        std::cout << "Displaed shape:" << std::endl 
             << doc.data() << std::endl;
    }

    virtual bool showDialog(const std::string& msg) override
    {
        std::cout << msg << " yes" <<  std::endl;
        return true;
    }

};

#endif