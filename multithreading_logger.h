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
    bool logQueueClear()
    {
        return _logQueue.empty();
    }

    void processQueue()
    {
        while (true)
        {            
            if (_logQueue.empty())
                _queueClearCondition.notify_all();

            std::unique_lock lock(_queueMutex);
            _queueCondition.wait(lock, [this] { return !_logQueue.empty() || _stop; });

            if (!_logQueue.empty())
            {
                _logger << _logQueue.front();
                _logQueue.pop();
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
        _queueCondition.notify_all();
        for (auto& worker : _workers)
        {
            if (worker.joinable())
                worker.join();
        }
    }

protected:
    std::condition_variable _queueClearCondition;

private:
    ILogger& _logger;
    std::queue<std::string> _logQueue;

    std::vector<std::thread> _workers;
    std::mutex _queueMutex;
    std::condition_variable _queueCondition;
    std::atomic<bool> _stop = false;
};

#endif