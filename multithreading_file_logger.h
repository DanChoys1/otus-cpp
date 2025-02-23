#ifndef MULTITHREADING_FILE_LOGGER_H
#define MULTITHREADING_FILE_LOGGER_H

#include "multithreading_logger.h"
#include "file_logger.h"

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

    bool open(const std::string& path)
    {
        close();
        return _fileLogger.open(path);
    }

    void close()
    {
        std::unique_lock lock(_closeMutex);
        while (!logQueueClear())        
            _queueClearCondition.wait(lock, [this] { return logQueueClear(); });

        _fileLogger.close();
    }

private:
    FileLogger _fileLogger;

    std::mutex _closeMutex;
};

#endif