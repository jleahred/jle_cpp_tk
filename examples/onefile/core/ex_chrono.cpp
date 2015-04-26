#include <iostream>

#include "core/chrono.h"
#include "core/misc.h"




int main(void)
{
    {
        JLE_COUT_TRACE(jle::chrono::now());
    }

    {
        JLE_COUT_TRACE(jle::chrono::now() +1s +333ms);
    }

    {
        JLE_COUT_TRACE(jle::chrono::now() + 10min);
    }

    {
        JLE_COUT_TRACE(std::chrono::hours(24*360) + 5s +333ms);
    }

    {
        auto start = jle::chrono::now() -1s -253ms;
        JLE_COUT_TRACE(jle::chrono::now() - start);
    }

    {
        JLE_COUT_TRACE(jle::chrono::make_from_date(jle::chrono::year{2015}, jle::chrono::month{4}, jle::chrono::day{13}));
    }

    {
        JLE_COUT_TRACE(jle::chrono::today());
    }

    {
        auto  a = jle::chrono::now();
        JLE_COUT_TRACE(a-a);
    }

    {
        auto a = jle::chrono::now();
        auto b = a + 1s +303ms;
        JLE_COUT_TRACE(a-b);
        JLE_COUT_TRACE(b-a);
        JLE_COUT_TRACE(a<b);
        JLE_COUT_TRACE(a>b);
        JLE_COUT_TRACE(a==b);
        JLE_COUT_TRACE(a!=b);
        JLE_COUT_TRACE(a<=b);
        JLE_COUT_TRACE(a>=b);
    }

}
