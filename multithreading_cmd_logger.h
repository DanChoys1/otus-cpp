#ifndef MULTITHREADING_CMD_LOGGER_H
#define MULTITHREADING_CMD_LOGGER_H

#include "multithreading_logger.h"
#include "cmd_logger.h"

class MultithreadingCmdLogger : public MultithreadingLogger
{
public:
    MultithreadingCmdLogger() :
        MultithreadingLogger(_cmdLogger, 1)
    {}

private:
    CmdLogger _cmdLogger;

    std::mutex _closeMutex;
};

#endif