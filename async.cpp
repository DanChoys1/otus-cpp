#include "async.h"
#include "command_parser.h"
#include "command_buffer.h"
#include "command_logger.h"

namespace async
{

class Handler
{
public:
    Handler(std::size_t block_size)
    {
        parser = std::make_shared<CommandsParser>(std::make_shared<CommandBuffer>(std::make_shared<CommandLogger>()), block_size);
        parserThread = std::thread([this]()
        {
            parser->parse(stream);
        });
    }

    void receive(const char* data, std::size_t size)
    {
        std::lock_guard lock(receiveMutex);
        stream << std::string(data, size);
    }

    ~Handler()
    {
        if (parserThread.joinable())
            parserThread.join();
    }

private:
    std::shared_ptr<CommandsParser> parser;
    std::stringstream stream;

    std::thread parserThread;

    std::mutex receiveMutex;
};

handle_t connect(std::size_t block_size)
{
    return reinterpret_cast<handle_t>(new Handler(block_size));
}

void receive(handle_t handle, const char* data, std::size_t size)
{
    Handler* handler = reinterpret_cast<Handler*>(handle);
    handler->receive(data, size);
}

void disconnect(handle_t handle)
{
    Handler* handler = reinterpret_cast<Handler*>(handle);
    delete handler;
}

}