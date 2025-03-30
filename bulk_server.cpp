#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>
#include "command_parser.h"
#include "command_buffer.h"
#include "command_logger.h"

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, std::shared_ptr<CommandsParser> parser)
        : socket_(std::move(socket)), parser_(std::move(parser)) {}

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string input(data_, length);
                    std::lock_guard<std::mutex> lock(session_mutex_);
                    std::stringstream stream(input);
                    parser_->parse(stream);
                    do_read();
                } else {
                    // Log error and end parsing
                    if (ec != boost::asio::error::eof) {
                        std::cerr << "Error: " << ec.message() << "\n";
                    }
                    std::lock_guard<std::mutex> lock(session_mutex_);
                    parser_->endParsing();
                }
            });
    }

    tcp::socket socket_;
    std::shared_ptr<CommandsParser> parser_;
    enum { max_length = 1024 };
    char data_[max_length];
    std::mutex session_mutex_; // Mutex for this session
};

class Server {
public:
    Server(boost::asio::io_context& io_context, short port, std::size_t bulk_size)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          parser_(std::make_shared<CommandsParser>(
              std::make_shared<CommandBuffer>(std::make_shared<CommandLogger>()), bulk_size)) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket), parser_)->start();
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor_;
    std::shared_ptr<CommandsParser> parser_;
};
