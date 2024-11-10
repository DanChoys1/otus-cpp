#ifndef COMMAND_LOGGER_H
#define COMMAND_LOGGER_H

#include <iostream>

#include "command.h"
#include "icommand_logger.h"
#include "file_logger.h"
#include "cmd_logger.h"

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
        _logTime = logTime;

        return *this;
    }

    void end() override 
    {
        _isFirstCommand = true;
        _logName.clear();
        _logTime = 0;
        _fileLogger.close();

        std::cout << std::endl;
    };

    ICommandLogger& operator<< (const Command& command) override
    {
        if (_isFirstCommand)
        {
            _isFirstCommand = false;

            _cmdLogger << _logName << ": ";
            _fileLogger.open(_logName + std::to_string(_logTime) + FileExtension);
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
    FileLogger _fileLogger;
    CmdLogger _cmdLogger;
    
    std::string _logName = "";
    time_t _logTime = 0;

    bool _isFirstCommand = true;
};

#endif