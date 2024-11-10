#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include <queue>
#include <memory>
#include <chrono>

#include "command.h"
#include "icommand_buffer.h"
#include "icommand_logger.h"

class CommandBuffer : public ICommandBuffer
{
public:
    CommandBuffer(std::shared_ptr<ICommandLogger> logger = std::make_shared<ICommandLogger>()) :
        _logger(logger)
    {}

    void setCommand(Command command) override
    {
        using namespace std::chrono;
        if (_commands.empty())
            _firsteCommandTime = system_clock::to_time_t(system_clock::now());
        _commands.push(command);
    }

    bool execut() override
    {
        if (_commands.empty())
            return true;
            
        _logger->start("bulk", _firsteCommandTime);

        while (!_commands.empty())
        {
            (*_logger) << _commands.front();
            _commands.pop();
        }

        _logger->end();
        
        return true;
    }

    size_t count() override { return _commands.size(); }

private:
    std::queue<Command> _commands;

    std::shared_ptr<ICommandLogger> _logger;

    time_t _firsteCommandTime = 0;

};

#endif