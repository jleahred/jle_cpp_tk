#include <iostream>
#include "support/chrono.h"



int main(void)
{
    {
        auto  n = jle::chrono::now();
        std::cout << n << std::endl;
    }

    {
        std::cout << jle::chrono::now() +1s +333ms << std::endl;
    }

    {
        jle::chrono::time_point  tp = jle::chrono::now() + 10min;
        std::cout << tp << std::endl;
    }

    {
        std::cout << std::chrono::hours(26) + 5s +333ms << std::endl;
    }

    {
        auto start = jle::chrono::now() -1s -253ms;
        std::cout << jle::chrono::now() - start << std::endl;
    }
}
