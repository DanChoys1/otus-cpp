#ifndef ICOMMAND_LOGGER_H
#define ICOMMAND_LOGGER_H

#include "command.h"

class ICommandLogger
{ 
public:
    virtual ICommandLogger& start([[maybe_unused]] std::string logName, [[maybe_unused]] time_t logTime)
    {
        /* Do nothing */ 
        return *this;
    }

    virtual void end() { /* Do nothing */ };

    virtual ICommandLogger& operator<< ([[maybe_unused]] const Command& command) 
    {
        /* Do nothing */ 
        return *this;
    }
};

#endif