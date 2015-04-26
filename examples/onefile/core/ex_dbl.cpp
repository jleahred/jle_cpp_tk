#include <iostream>
#include "core/dbl.h"
#include "core/misc.h"



int main()
{
    //  this is the reason to have jle::dbl
    {
        std::cout << "Next is wrong..." << std::endl;
        JLE_COUT_TRACE((1*(0.5-0.4-0.1) < 0.)  == false)

        std::cout << "Next is OK..." << std::endl;
        JLE_COUT_TRACE((jle::dbl(1*(0.5-0.4-0.1)) < 0)  == false)

        std::cout << "We can use == ..." << std::endl;
        JLE_COUT_TRACE((jle::dbl(1*(0.5-0.4-0.1)) == 0)  == true)
    }


    //  convert from double to  jle::dbl
    {
        auto d = jle::dbl(3.1415926535);
        JLE_COUT_TRACE(d);
    }


    //  operations
    {
        auto d = jle::dbl(3.1415926535);
        JLE_COUT_TRACE(d + 10.);
    }

    {
        auto d = jle::dbl(3.1415926535);
        JLE_COUT_TRACE(d - 10.);
    }

    {
        auto d = jle::dbl(3.1415926535);
        JLE_COUT_TRACE(d * 10.);
    }

    {
        auto d = jle::dbl(3.1415926535);
        JLE_COUT_TRACE(d / 10.);
    }

    {
        auto d = jle::dbl(3.1415926535);
        JLE_COUT_TRACE(d += 10.);
        JLE_COUT_TRACE(d == 3.1415926535+10.);
    }

    {
        auto d = jle::dbl(3.1415926535);
        JLE_COUT_TRACE(d -= 10.);
        JLE_COUT_TRACE(d);
        JLE_COUT_TRACE(d == 3.1415926535-10.);
    }

    {
        auto d = jle::dbl(3.1415926535);
        JLE_COUT_TRACE(d *= 10.);
        JLE_COUT_TRACE(d);
        JLE_COUT_TRACE(d == 3.1415926535*10.);
    }

    {
        auto d = jle::dbl(3.1415926535);
        JLE_COUT_TRACE(d /= 10.);
        JLE_COUT_TRACE(d);
        JLE_COUT_TRACE(d == 3.1415926535/10.);
    }


    //  convert from jle::dbl to double
    //  do you realy need it?
    {
        auto d = jle::dbl(3.1415926535);
        JLE_COUT_TRACE(static_cast<double>(d));
    }

    return 0;
}
