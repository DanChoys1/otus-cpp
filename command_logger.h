#ifndef COMMAND_LOGGER_H
#define COMMAND_LOGGER_H

#include <iostream>

#include "command.h"
#include "icommand_logger.h"
#include "multithreading_file_logger.h"
#include "multithreading_cmd_logger.h"
#include "uuid_generator.h"

namespace
{
    const std::string FileExtension = ".log";
}

class CommandLogger : public ICommandLogger
{
public:
    ICommandLogger& start(std::string logName, time_t logTime) override
    {
        _isFirstCommand = true;
        _logName = std::move(logName);
        _logPostfix = std::to_string(logTime) + '_' + generateUUID();

        return *this;
    }

    void end() override 
    {
        _isFirstCommand = true;
        _logName.clear();
        _logPostfix.clear();
        _fileLogger.close();

        std::cout << std::endl;
    };

    ICommandLogger& operator<< (const Command& command) override
    {
        if (_isFirstCommand)
        {
            _isFirstCommand = false;

            _cmdLogger << _logName << ": ";
            _fileLogger.open(_logName + _logPostfix + FileExtension);
        }
        else
        {
            _cmdLogger << ", ";
            _fileLogger << ", ";
        }

        _cmdLogger << command;
        _fileLogger << command;
        return *this;
    }

private:
    MultithreadingFileLogger _fileLogger;
    MultithreadingCmdLogger _cmdLogger;
    
    std::string _logName = "";
    time_t _logTime = 0;
    std::string _logPostfix;

    bool _isFirstCommand = true;
};

#endif