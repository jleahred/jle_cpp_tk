#include "core/cont/vector.hpp"
#include "core/cont/list.hpp"
#include "core/cont/map.hpp"

#include <iostream>


#include "core/alarm.h"




int main(void)
{
    {
        jle::vector<int> vint;

        vint.push_back(5);
        vint.push_back(4);
        vint.push_back(3);
        vint.push_back(2);
        vint.push_back(1);

        std::cout << vint.size() << std::endl;
    }



    {
        jle::map<int, std::string>  mis;
        mis[55] = "555555";
        mis[33] = "333333";
        mis[44] = "444444";

        std::cout << mis.size() << std::endl;
    }

    {
        jle::list<std::string>  li;
        li.push_back("00000000");
        li.push_back("1111111");
        li.push_back("22222222");

        std::cout << li.size() << std::endl;
    }
}


void jle::alarm_msg(const jle::alarm& al)
{
    std::cout << "ouch...: "  << al << std::endl;

}
