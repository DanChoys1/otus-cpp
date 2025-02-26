#ifndef MULTITHREADING_LOGGER_H
#define MULTITHREADING_LOGGER_H

#include "ilogger.h"

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>

class MultithreadingLogger
{
public:
    MultithreadingLogger(ILogger& logger, size_t threadCount) :
        _logger(logger)
    {
        _workers.reserve(threadCount);
        for (size_t i = 0; i < threadCount; ++i)
        {
            _workers.emplace_back(&MultithreadingLogger::processQueue, this);
        }
    }

    ~MultithreadingLogger()
    {
        stop();
    }

    MultithreadingLogger& operator<< (const std::string& msg)
    {
        {
            std::unique_lock lock(_queueMutex);
            _logQueue.push(msg);
        }
        _queueCondition.notify_one();

        return *this;
    }

protected:
    bool isLogQueueClear()
    {
        return _logQueue.empty();
    }

private:
    void processQueue()
    {
        while (!_isStop || !isLogQueueClear())
        {
            std::unique_lock lock(_queueMutex);
            _queueCondition.wait(lock, [this] { return !isLogQueueClear() || _isStop; });

            if (!isLogQueueClear())
            {
                _logger << _logQueue.front();
                _logQueue.pop();
            }
            
            if (isLogQueueClear())
            {
                _queueClearCondition.notify_all();
            }
        }
    }

    void stop()
    {
        _isStop = true;
        _queueCondition.notify_all();
        for (auto& worker : _workers)
        {
            if (worker.joinable())
                worker.join();
        }
    }

protected:
    std::mutex _queueMutex;
    std::condition_variable _queueClearCondition;
    
private:
    ILogger& _logger;
    std::queue<std::string> _logQueue;
    
    std::vector<std::thread> _workers;
    std::condition_variable _queueCondition;
    std::atomic<bool> _isStop = false;
};

#endif