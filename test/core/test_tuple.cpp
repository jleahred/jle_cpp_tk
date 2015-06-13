#include "core/test.h"

#include "core/tuple.hpp"
#include "core/misc.h"


void test_ostring(void)
{
    JLE_TEST_INIT

        auto t1 = jle::tuple<int, std::string>{1, "one"};
        JLE_TEST_ASSERT(JLE_SS(t1)  == std::string{"t(1, one)"})
}



int main(void)
{

    JLE_TEST_FILE

        test_ostring();

    JLE_TEST_REPORT
}



namespace jle {

    void alarm_msg(const jle::alarm& al)
    {
        std::cout << al << std::endl;
        exit(-1);
    }

}
