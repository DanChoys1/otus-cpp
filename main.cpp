#include "async.h"

int main()
{
    std::size_t bulk = 5;
    auto h1 = async::connect(bulk);
    auto h2 = async::connect(bulk);

    async::receive(h1, "1", 1);
    async::receive(h2, "1\n", 2);
    async::receive(h1, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h1, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h1);
    async::disconnect(h2);

    return 0;
}
