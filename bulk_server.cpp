#include "bulk_server.h"

#include <iostream>
#include <sstream>
#include <atomic>
#include "command_parser.h"
#include "command_buffer.h"
#include "command_logger.h"

namespace
{
class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket, std::shared_ptr<CommandsParser> parser, std::atomic_flag& dynamicBlockFlag)
        : _socket(std::move(socket)),
          _parser(parser),
          _dynamicBlockFlag(dynamicBlockFlag) 
    {}

    void start() { doRead(); }

private:
    void doRead()
    {
        auto self(shared_from_this());
        boost::asio::async_read_until(_socket, _buffer, '\n',
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (ec)
                {
                    if (ec != boost::asio::error::eof)
                        std::cerr << ec.message() << std::endl;
                    _parser->endParsing();
                    return;
                }

                std::istream stream(&_buffer);
                std::string input;
                while (std::getline(stream, input))
                {
                    if (_parser->isDynamicStartCommand(input))
                    {
                        do
                        {
                            _dynamicBlockFlag.wait(false);
                        } 
                        while (_dynamicBlockFlag.test_and_set());
                    }
                    else if (_parser->isDynamicEndCommand(input))
                    {
                        _dynamicBlockFlag.clear(std::memory_order_release);
                        _dynamicBlockFlag.notify_all();
                    }

                    _parser->parse(input);
                }

                doRead();
            });
    }

private:
    tcp::socket _socket;
    std::shared_ptr<CommandsParser> _parser;
    boost::asio::streambuf _buffer;
    std::atomic_flag& _dynamicBlockFlag;
};

}

Server::Server(boost::asio::io_context& ioContext, int port, std::size_t bulkSize)
    : _acceptor(ioContext, tcp::endpoint(tcp::v4(), port)),
        _parser(std::make_shared<CommandsParser>(
                    std::make_shared<CommandBuffer>(
                        std::make_shared<CommandLogger>()), 
                bulkSize))
{
    _dynamicBlockFlag.clear();
    doAccept();
}

void Server::doAccept()
{
    _acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
                std::make_shared<Session>(std::move(socket), _parser, _dynamicBlockFlag)->start();
            doAccept();
        });
}
