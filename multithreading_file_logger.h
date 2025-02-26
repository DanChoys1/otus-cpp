#ifndef MULTITHREADING_FILE_LOGGER_H
#define MULTITHREADING_FILE_LOGGER_H

#include "multithreading_logger.h"
#include "file_logger.h"
#include <iostream>

class MultithreadingFileLogger : public MultithreadingLogger
{
public:
    MultithreadingFileLogger() :
        MultithreadingLogger(_fileLogger, 2)
    {}

    ~MultithreadingFileLogger()
    {
        close();
    }

    MultithreadingFileLogger& operator<< (const std::string& msg)
    {
        MultithreadingLogger::operator<< (msg);
        return *this;
    }

    bool open(const std::string& path)
    {
        close();
        return _fileLogger.open(path);
    }

    void close()
    {
        std::unique_lock lock(_queueMutex);
        while (!isLogQueueClear())        
            _queueClearCondition.wait(lock, [this] { return isLogQueueClear(); });
        
        _fileLogger.close();
    }

private:
    FileLogger _fileLogger;
};

#endif