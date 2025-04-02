#include "bulk_server.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: bulk_server <port> <bulk_size>\n";
        return 1;
    }

    try
    {
        boost::asio::io_context ioContext;
        Server server(ioContext, std::stoi(argv[1]), std::stoul(argv[2]));
        ioContext.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
