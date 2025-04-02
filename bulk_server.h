#ifndef BULK_SERVER_H
#define BULK_SERVER_H

#include <boost/asio.hpp>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

class CommandsParser;

class Server
{
public:
    Server(boost::asio::io_context& ioContext, int port, std::size_t bulkSize);

private:
    void doAccept();

private:
    tcp::acceptor _acceptor;
    std::shared_ptr<CommandsParser> _parser;
    std::atomic_flag _dynamicBlockFlag = ATOMIC_FLAG_INIT;
};


#endif // BULK_SERVER_H
