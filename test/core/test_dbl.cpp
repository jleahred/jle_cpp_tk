#include "core/dbl.h"
#include "core/test.h"


void test_common_mistake(void)
{
    JLE_TEST_INIT

    //  tipical ieee754 programmer mistake
    {
        JLE_TEST_ASSERT((jle::dbl(1.*(0.5-0.4-0.1)) == 0.)  == true)
        JLE_TEST_ASSERT((jle::dbl(1.*(0.5-0.4-0.1)) >  0.)  == false)
        JLE_TEST_ASSERT((jle::dbl(1.*(0.5-0.4-0.1)) <  0.)  == false)
        JLE_TEST_ASSERT((jle::dbl(1.*(0.5-0.4-0.1)) >= 0.)  == true)
        JLE_TEST_ASSERT((jle::dbl(1.*(0.5-0.4-0.1)) <= 0.)  == true)

        //  JLE_TEST_ASSERT(((1*(0.5-0.4-0.1)) <  0.)  == false)    using ieee754 (double) fails
    }
}


void test_operations(void)
{
    JLE_TEST_INIT

    const double pi = 3.1415926535;
    //  operations
    {
        auto d = jle::dbl(pi);
        JLE_TEST_ASSERT(d + 10. ==  pi+10.);
    }

    {
        auto d = jle::dbl(pi);
        JLE_TEST_ASSERT(d - 10.  ==  pi-10.);
    }

    {
        auto d = jle::dbl(pi);
        JLE_TEST_ASSERT(d * 10.  == pi*10.);
    }

    {
        auto d = jle::dbl(pi);
        JLE_TEST_ASSERT(d / 10. == pi/10.);
    }

    {
        auto d = jle::dbl(pi);
        d += 10.;
        JLE_TEST_ASSERT(d == pi+10.);
    }

    {
        auto d = jle::dbl(pi);
        d -= 10.;
        JLE_TEST_ASSERT(d == pi-10.);
    }

    {
        auto d = jle::dbl(pi);
        d *= 10.;
        JLE_TEST_ASSERT(d == pi*10.);
    }

    {
        auto d = jle::dbl(pi);
        d /= 10.;
        JLE_TEST_ASSERT(d == pi/10.);
    }
}


void test_get_decimals(int sign)
{
    JLE_TEST_INIT

    int ndec=1;
    for(int p10=10; p10 < 100'000'000; p10*=10, ++ndec)
    {
        for(int i=0; i<1'000'000; ++i)
        {
            for(int j=1; j<10; ++j)
            {
                auto d = jle::dbl{double(i*10)/double(p10) + double(j)/double(p10) * sign};
                JLE_TEST_ASSERT_NO_DOT(jle::get_decimals10(d)  ==  ndec);
            }
        }
        JLE_TEST_DOT
    }
}

int main(void)
{

    JLE_TEST_FILE

        test_common_mistake();
        test_operations();
        test_get_decimals(1);
        test_get_decimals(-1);

    JLE_TEST_REPORT
}
