#include <iostream>

#include "test.h"
#include "core/alarm.h"



int main()
{

    auto simple = t_simple{ 1, "one", "two" };

    std::cout << simple << std::endl;

    return 0;
}


void jle::alarm_msg(const jle::alarm&  al)
{
    std::cerr << al << std::endl;
}
