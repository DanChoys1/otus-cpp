#ifndef CMD_LOGGER_H
#define CMD_LOGGER_H

#include <iostream>
#include "ilogger.h"

class CmdLogger : public ILogger
{
public:
    CmdLogger() :
        ILogger(std::cout)
    {}
    
};

#endif