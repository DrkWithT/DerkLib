#include <print>
#include "foo.hpp"

int main()
{
    if (DerkLib::foo(10, 20) != 15) {
        return 1;
    }

    std::print("OK\n");
}