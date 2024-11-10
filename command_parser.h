#ifndef COMMAN_PARSER_H
#define COMMAN_PARSER_H

#include <iostream>
#include <memory>

#include "icommand_buffer.h"

class CommandsParser
{
public:
    CommandsParser(std::shared_ptr<ICommandBuffer> commBuff, int blockSize = 1) :
        _commBuff(commBuff), _blockSize(blockSize)
    {}

    void parse(std::istream& stream)
    {
        std::string line;
        while(std::getline(stream, line))
        {
            if (line == "{")
                newDynamicBlock();
            else if (line == "}")
                endDynamicBlock();
            else
                newCommand(line);
        }

        endParsing();
    }

private:
    void newDynamicBlock()
    {
        if (++_dynamicBlocks == 1)
        {
            _commBuff->execut();
        }
    }

    void endDynamicBlock()
    {
        if (--_dynamicBlocks == 0)
        {
            _commBuff->execut();
        }
    }

    void newCommand(std::string line)
    {
        Command command { move(line) };
        _commBuff->setCommand(command);
        if (_commBuff->count() == _blockSize && _dynamicBlocks == 0)
            _commBuff->execut();
    }

    void endParsing()
    {
        if (_commBuff->count() > 0 && _dynamicBlocks == 0)
            _commBuff->execut();
    }

private:
    std::shared_ptr<ICommandBuffer> _commBuff;

    int _blockSize = 1;
    int _dynamicBlocks = 0;
    
};

#endif