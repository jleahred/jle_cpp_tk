#include <iostream>
#include <tuple>

#include "core/string.h"


int main()
{
    {
        auto result = jle::s_try_s2d("3.1415926535", 0.);
        if(std::get<1>(result))
            std::cout <<  std::get<0>(result) << std::endl;
    }

    {
        auto result = jle::s_try_s2dur("3m 4s 132ms", jle::chrono::duration{0});
        if(std::get<1>(result))
            std::cout <<  std::get<0>(result) << std::endl;
    }

}
