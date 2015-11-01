#include <iostream>

#include "test.h"
#include "core/alarm.h"
#include "core/tuple.hpp"



int main()
{
    auto simple = idl::pr::t_simple{ 1, "one", "two" };

    std::cout << simple << std::endl;


    idl::pr::t_tuple  t{1, "one" };
    std::cout << t << std::endl;

    idl::pr::t_tuple2  t2{2, "two" };
    std::cout << t2 << std::endl;

    return 0;
}


void jle::alarm_msg(const jle::alarm&  al)
{
    std::cerr << al << std::endl;
}
