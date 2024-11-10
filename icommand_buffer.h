#ifndef ICOMMAND_BUFFER_H
#define ICOMMAND_BUFFER_H

#include "command.h"

class ICommandBuffer
{
public:
    virtual void setCommand(Command command) = 0;
    virtual bool execut() = 0;

    virtual size_t count() = 0;
};

#endif