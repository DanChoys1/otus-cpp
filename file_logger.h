#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include <fstream>
#include "ilogger.h"

class FileLogger : public ILogger
{
public:
    FileLogger() :
        ILogger(_outFile)
    {}

    bool open(const std::string& path)
    {
        if (_filePath == path && _outFile.is_open())
            return true;
            
        if (_outFile.is_open()) 
            _outFile.close();

        _filePath = path;
        _outFile.open(_filePath, std::ios_base::binary | std::ios_base::out);

        return _outFile.is_open();
    }

    void close()
    {
        _outFile.close();
    }

private:
    std::ofstream _outFile;
    std::string _filePath;
    
};

#endif