#ifndef ICOMMAND_LOGGER_H
#define ICOMMAND_LOGGER_H

#include "command.h"

class ICommandLogger
{ 
public:
    virtual ICommandLogger& start(std::string logName, time_t logtime)
    {
        /* Do nothing */ 
        return *this;
    }

    virtual void end() { /* Do nothing */ };

    virtual ICommandLogger& operator<< (const Command& command) 
    {
        /* Do nothing */ 
        return *this;
    }
};

#endif