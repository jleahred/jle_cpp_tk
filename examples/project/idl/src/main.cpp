#include <iostream>

#include "test.h"
#include "core/alarm.h"
#include "core/tuple.hpp"

struct  t_pr { //: public std::tuple<int, int> {
    std::tuple<int, int>  tuple;
    t_pr(int a, int b)  : tuple{std::make_tuple(a, b)}  {};
    std::tuple<int, int>& operator*() { return  tuple;  }
};


struct  t_pr2 { //: public std::tuple<int, int> {
    std::tuple<int, int>  tuple;
    t_pr2(int a, int b)  : tuple{std::make_tuple(a, b)}  {};
    std::tuple<int, int>& operator*() { return  tuple;  }
};



int main()
{
    t_pr pr{1,2};
    t_pr2 pr2{1,2};

    JLE_COUT_TRACE(std::get<0>(*pr));
    if(*pr==*pr2)
        std::cout << "yeahh" << std::endl;

    auto simple = idl::pr::jt_simple{ 1, "one", "two" };

    std::cout << simple << std::endl;

    return 0;
}


void jle::alarm_msg(const jle::alarm&  al)
{
    std::cerr << al << std::endl;
}
