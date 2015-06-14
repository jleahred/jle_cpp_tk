#include <iostream>

#include "core/tuple.hpp"
#include "core/alarm.h"





int main(void)
{
    auto t1 = std::tuple<int, std::string>{1, "one"};
    auto t2 = jle::tuple<int, std::string>{2, "two"};

    if(t1 == t2)
        std::cout << "yes" << std::endl;
    if(t1 < t2)
        std::cout << "yes" << std::endl;


    JLE_COUT_TRACE(t1);
    JLE_COUT_TRACE(t2);

    std::cout << t1 << std::endl;

    return 0;
}



namespace jle {

    void alarm_msg(const jle::alarm& al)
    {
        std::cout << al << std::endl;
    }

}
