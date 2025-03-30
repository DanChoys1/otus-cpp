#include "bulk_server.cpp"

#include <iostream>
#include <sstream>
#include <boost/asio.hpp>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: bulk_server <port> <bulk_size>\n";
        return 1;
    }

    try {
        boost::asio::io_context io_context;
        short port = std::stoi(argv[1]);
        std::size_t bulk_size = std::stoul(argv[2]);

        Server server(io_context, port, bulk_size);
        // Server server(io_context, 9000, 3);
        io_context.run();
        // std::this_thread::sleep_for(std::chrono::seconds(100000));
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
