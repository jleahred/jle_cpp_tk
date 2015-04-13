#include <iostream>
#include "support/chrono.h"



int main(void)
{
    {
        auto  n = jle::chrono::now();
        std::cout << "now ->  " << n << std::endl;
    }

    {
        std::cout << "now + 1s+333ms ->  " << jle::chrono::now() +1s +333ms << std::endl;
    }

    {
        jle::chrono::time_point  tp = jle::chrono::now() + 10min;
        std::cout << "now +10min ->  " << tp << std::endl;
    }

    {
        std::cout << "(24*360)h + 5s + 333ms ->  " << std::chrono::hours(24*360) + 5s +333ms << std::endl;
    }

    {
        auto start = jle::chrono::now() -1s -253ms;
        std::cout << "time between now and start ->  " << jle::chrono::now() - start << std::endl;
    }

    {
        using namespace jle::chrono::t;
        std::cout << "make_from_date(year{2015}, month{4}, day{13}) ->  " << jle::chrono::make_from_date(year{2015}, month{4}, day{13}) << std::endl;
    }

    {
        std::cout << "today ->  " << jle::chrono::today() << std::endl;
    }
}
