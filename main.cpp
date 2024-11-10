#include "command_parser.h"
#include "command_buffer.h"
#include "command_logger.h"

int main(int argc, char* argv[])
{    
    int blockSize = argc > 1 ? atoi(argv[1]) : 1;
    CommandsParser parser(std::make_shared<CommandBuffer>(std::make_shared<CommandLogger>()), blockSize);
    parser.parse(std::cin);

    return 0;
}
