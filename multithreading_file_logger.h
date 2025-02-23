#ifndef MULTITHREADING_FILE_LOGGER_H
#define MULTITHREADING_FILE_LOGGER_H

#include "file_logger.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>

namespace
{
    const size_t ThreadCount = 2;
}

class MultithreadingFileLogger
{
public:
    MultithreadingFileLogger()
    {
        _workers.reserve(ThreadCount);
        for (size_t i = 0; i < ThreadCount; ++i)
        {
            _workers.emplace_back(&MultithreadingFileLogger::processQueue, this);
        }
    }

    ~MultithreadingFileLogger()
    {
        stop();
    }

    bool open(const std::string& path)
    {
        close();
        return _fileLogger.open(path);
    }

    void close()
    {
        std::unique_lock<std::mutex> lock(_closeMutex);
        _close = true;
        _closeCondition.wait(lock, [this] { return _logQueue.empty(); });

        _fileLogger.close();
        _close = false;
    }

    MultithreadingFileLogger& operator<< (const std::string& msg)
    {
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _logQueue.push(msg);
        }
        _queueCondition.notify_one();

        return *this;
    }

private:
    void processQueue()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _queueCondition.wait(lock, [this] { return !_logQueue.empty() || _stop; });

            if (!_logQueue.empty())
            {
                _fileLogger << _logQueue.front();
                _logQueue.pop();
            }
            else if (_close)
            {
                _closeCondition.notify_all();
            }
            else if (_stop)
            {
                break;
            }
        }
    }

    void stop()
    {
        _stop = true;
        close();
        _queueCondition.notify_all();
        for (auto& worker : _workers)
        {
            if (worker.joinable())
                worker.join();
        }
    }

private:
    FileLogger _fileLogger;
    std::queue<std::string> _logQueue;

    std::vector<std::thread> _workers;
    std::mutex _queueMutex;
    std::condition_variable _queueCondition;
    std::atomic<bool> _stop = false;

    std::mutex _closeMutex;
    std::condition_variable _closeCondition;
    std::atomic<bool> _close = false;
};

#endif