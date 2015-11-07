#include <iostream>

#include "test.h"
#include "core/alarm.h"
#include "core/tuple.hpp"




int main()
{
    auto d1 = jle::optional<jle::dbl>{1.0};
    auto d2 = jle::optional<jle::dbl>{1.0};
    std::cout << int(d1 == d2) << std::endl;

    {   //  record
        auto simple = idl::pr::t_simple{ 1, "one", "two" };

        std::cout << simple << std::endl;
    }


    {   //  tuple
        idl::pr::t_tuple  t{1, "one" };
        std::cout << t << std::endl;

        idl::pr::t_tuple2  t2{2, "two" };
        std::cout << t2 << std::endl;
    }

    {   //  enum
        using namespace  idl::pr;
        auto color = t_color{t_color::red};
        std::cout << color << std::endl;
    }

    return 0;
}


void jle::alarm_msg(const jle::alarm&  al)
{
    using ::operator<<;
    std::cerr << al << std::endl;
}
