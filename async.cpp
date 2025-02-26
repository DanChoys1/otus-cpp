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
    }

    void receive(const char* data, std::size_t size)
    {
        std::lock_guard lock(parseMutex);
        std::stringstream stream;
        stream << std::string(data, size);
        parser->parse(stream);
    }

    ~Handler()
    {
        std::lock_guard lock(parseMutex);
        parser->endParsing();
    }

private:
    std::shared_ptr<CommandsParser> parser;
    std::mutex parseMutex;
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